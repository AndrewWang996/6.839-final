#pragma once
#include <vector>
#include <iostream>
#include <fstream>

#include "generate_commands.hpp"
#include <Eigen/Eigen>


namespace fab_translation {

    class GCodeConverter{

    private:
        static void WriteGCodeHeader(std::ofstream& gcode_filestream){
            gcode_filestream << "G28 ;\n"; //home head
            gcode_filestream << "G90 ;\n"; //absolute positioning mode
            gcode_filestream << "M106 S255 ;\n"; //fan on!
            gcode_filestream << "M109 S190 T0 ;\n"; //heat head to 190 degrees, then wait until it reaches that point.
            gcode_filestream << "M190 S50 ;\n"; //heat bed to 50 degrees, if possible, and wait
        }

        static void WriteGCodeFooter(std::ofstream& gcode_filestream) {
            gcode_filestream << "M104 S0 ;\n"; //cool hot end
            gcode_filestream << "M140 S0 ; \n"; //cool printbed
            gcode_filestream << "M107 ; \n"; //fan off
            gcode_filestream << "G1 X0 Y180 F9000; \n"; //move the bed for easy retrieval
        }

        static void MoveToPosition(std::ofstream& gcode_filestream, const double speed, const Eigen::Vector3d& target){
            //TODO: some sort of string builder for efficiency
            std::string code = "G1 X";
            code.append(std::to_string(target.x()));
            code.append(" Y");
            code.append(std::to_string(target.y()));
            code.append(" Z");
            code.append(std::to_string(target.z()));
            code.append(" F");
            code.append(std::to_string(speed));
            code.append(" ;\n");
            gcode_filestream << code;
        }

        static void WriteExtrudeSingleSegment(std::ofstream& gcode_filestream, const double speed, Eigen::Vector3d& p0, Eigen::Vector3d& p1){
            //TODO: modularize above code and this code somehow
            std::string code = "G1 X";

            //calculate extrude amount:
            double extrude_amt = (p1 - p0).norm();

            code.append(std::to_string(p1.x()));
            code.append(" Y");
            code.append(std::to_string(p1.y()));
            code.append(" Z");
            code.append(std::to_string(p1.z()));
            code.append(" E");
            code.append(std::to_string(extrude_amt));
            code.append(" F");
            code.append(std::to_string(speed));
            code.append(" ;\n");
            gcode_filestream << code;
        }

        static void WriteExtrudeSegment(std::ofstream& gcode_filestream, const double speed, const std::vector<Eigen::Vector3d>& targets){

            //assume already at starting point.

            Eigen::Vector3d last_target = targets[0];
            for (uint i = 1; i < targets.size(); ++i){
                Eigen::Vector3d target = targets[i];
                WriteExtrudeSingleSegment(gcode_filestream, speed, last_target, target);
                last_target = target;
            }
        }


    public:
        //if output_file is NULL then this goes to the virtual machine
        static void ConvertToGCode(std::vector<std::vector<Eigen::Vector3d>>& segments, const std::string * output_file_name){
            const double default_speed = 600; //600 mm / min or 1cm per second

            if (output_file_name == nullptr){
                //send to virtual printer - send each segment, and move to beginning of segment after each one.
                for (std::vector<Eigen::Vector3d> segment : segments){
                    network_communication::GenerateCommands::MoveCommand(std::vector<Eigen::Vector3d>{segment[0]}, false); //move to beginning
                    for (Eigen::Vector3d point : segment){
                        network_communication::GenerateCommands::MoveCommand(std::vector<Eigen::Vector3d>{point}, true); //handle segment
                    }
                }
            }else{
                //write to gcode

                std::ofstream gcode_file;
                gcode_file.open (output_file_name->c_str());
                WriteGCodeHeader(gcode_file);
                for (std::vector<Eigen::Vector3d> segment : segments){
                    MoveToPosition(gcode_file, default_speed, segment[0]);
                    WriteExtrudeSegment(gcode_file, default_speed, segment);
                }

                WriteGCodeFooter(gcode_file);
                gcode_file.close();


            }
        }

    };







}