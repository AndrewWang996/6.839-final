#pragma once


#include "fossfab.pb.h"
#include <Eigen/Eigen>
#include <tuple>
#include <iostream>
#include "server.hpp"


namespace network_communication{


    class GenerateCommands{

        typedef std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> triangle;

    private:
        static char * ConvertMessageToBuffer(const FossFab::Command& command, int& size){
            size = command.ByteSize();
            void * buffer = malloc(size);
            command.SerializeToArray(buffer, size);
            char * char_buffer = (char *)buffer;


            std::cout << size << std::endl;

            return char_buffer;
        }

    public:
        static void MoveCommand(const std::vector<Eigen::Vector3d>& poses, bool extrude){

            FossFab::Command command;

            command.set_command_code(FossFab::Command_CommandCode::Command_CommandCode_MOVE);

            for (Eigen::Vector3d pose : poses){
                FossFab::HeadPosition  * head_position = command.add_positions();
                FossFab::Vector3 * pos = head_position->mutable_pos();
                pos->set_x(pose.x());
                pos->set_y(pose.y());
                pos->set_z(pose.z());
            }
            command.set_extrude(extrude);

            int size;
            char * bytes_to_send = ConvertMessageToBuffer(command, size);
            network_communication::socket_send(bytes_to_send, (uint)size);
        }


        static void PathCommand(const std::vector<Eigen::Vector3d>& poses){

            FossFab::Command command;

            command.set_command_code(FossFab::Command_CommandCode::Command_CommandCode_RENDER_PATH);

            for (Eigen::Vector3d pose : poses){
                FossFab::HeadPosition  * head_position = command.add_positions();
                FossFab::Vector3 * pos = head_position->mutable_pos();
                pos->set_x(pose.x());
                pos->set_y(pose.y());
                pos->set_z(pose.z());
            }

            int size;
            char * bytes_to_send = ConvertMessageToBuffer(command, size);
            network_communication::socket_send(bytes_to_send, (uint)size);
        }



        static char * ResetCommand(){
            FossFab::Command command;
            command.set_command_code(FossFab::Command_CommandCode::Command_CommandCode_RESET);
            int size;
            char * bytes_to_send = ConvertMessageToBuffer(command, size);
            network_communication::socket_send(bytes_to_send, (uint)size);
        }


        static void SlicesCommand(const std::vector<std::vector<Eigen::Vector3d>>& slices){
            FossFab::Command command;
            command.set_command_code(FossFab::Command_CommandCode::Command_CommandCode_RENDER_SLICES);

            for (std::vector<Eigen::Vector3d> slice_data : slices){
                FossFab::Slice * slice = command.add_slices();
                for (Eigen::Vector3d pose : slice_data){
                    FossFab::Vector3 * pos = slice->add_positions();
                    pos->set_x(pose.x());
                    pos->set_y(pose.y());
                    pos->set_z(pose.z());
                }
            }
            int size;
            char * bytes_to_send = ConvertMessageToBuffer(command, size);
            network_communication::socket_send(bytes_to_send, (uint)size);
        }


        //TODO: unordered set?
        static void MeshCommand(const std::vector<triangle> triangles){
            FossFab::Command command;
            command.set_command_code(FossFab::Command_CommandCode::Command_CommandCode_RENDER_MESH);
            FossFab::Mesh * mesh = command.mutable_mesh();
            for (triangle tri : triangles){
                FossFab::Triangle * triangle_to_add = mesh->add_triangles();

                FossFab::Vector3 * vertex1 = triangle_to_add->mutable_vertex1();
                FossFab::Vector3 * vertex2 = triangle_to_add->mutable_vertex2();
                FossFab::Vector3 * vertex3 = triangle_to_add->mutable_vertex3();

                vertex1->set_x(std::get<0>(tri).x());
                vertex1->set_y(std::get<0>(tri).y());
                vertex1->set_z(std::get<0>(tri).z());

                vertex2->set_x(std::get<1>(tri).x());
                vertex2->set_y(std::get<1>(tri).y());
                vertex2->set_z(std::get<1>(tri).z());

                vertex3->set_x(std::get<2>(tri).x());
                vertex3->set_y(std::get<2>(tri).y());
                vertex3->set_z(std::get<2>(tri).z());
            }
            int size;
            char * bytes_to_send = ConvertMessageToBuffer(command, size);
            network_communication::socket_send(bytes_to_send, (uint)size);
        }


    };






}