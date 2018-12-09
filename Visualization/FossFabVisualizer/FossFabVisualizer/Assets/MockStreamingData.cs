using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Text;
using Google.Protobuf;



using System.Net;
using System.Net.Sockets;



public class MockStreamingData : MonoBehaviour
{


    TcpClient client;
    Socket s;





    int time = 0;
    static bool clientConnected = false;


    // Use this for initialization
    void Start()
    {


    }

    public void Initialize()
    {






    }


    // Update is called once per frame
    void Update()
    {

        Vector3 position = new Vector3(Mathf.Cos(Time.time), Mathf.Sin(Time.time), 0.0f);

        if (!clientConnected)
        {
            client = new TcpClient();
            client.Connect("127.0.0.1", 11000);
            clientConnected = true;
            s = client.Client;
        }





        if (clientConnected)
        {

            //mode 0:


            ++time;

            FossFab.Command command = new FossFab.Command();

            if (false)
            {
                command.CommandCode = FossFab.Command.Types.CommandCode.RenderSlices;

                
                FossFab.Slice slice = new FossFab.Slice();


                

                    
                    for (int i = 0; i < 20; ++i)
                    {
                        FossFab.Vector3 pos = new FossFab.Vector3();
                        pos.X = Mathf.Cos((float)i / 5.0f) *  0.1f;
                        pos.Y = 0.1f;
                        pos.Z = Mathf.Sin((float)i / 5.0f) * 0.1f;


                        
                        slice.Positions.Add(pos);
                    }

                    command.Slices.Add(slice);

                Debug.Log(command.CalculateSize());


            }
            if (false)
            {

                command.CommandCode = FossFab.Command.Types.CommandCode.RenderPath;
                FossFab.HeadPosition headPosition = new FossFab.HeadPosition();
                headPosition.Pos = new FossFab.Vector3();

                if (time % 100 == 0)
                {
                    for (int i = 0; i < 10000; ++i)
                    {
                        headPosition.Pos.X = position.x;
                        headPosition.Pos.Y = position.y * i / 1000.0f;
                        headPosition.Pos.Z = position.z;

                        command.Positions.Add(headPosition);
                    }

                    command.Extrude = true;
                }

            }
            if (true)
            {
                if (time % 200 == 0) { 
                    command.CommandCode = FossFab.Command.Types.CommandCode.Move;

                    for (int i = 0; i < 10; ++i) { 
                        FossFab.HeadPosition headPosition = new FossFab.HeadPosition();
                        headPosition.Pos = new FossFab.Vector3();
                        headPosition.Pos.X = i;
                        headPosition.Pos.Y = i;
                        headPosition.Pos.Z = i;
                        command.Positions.Add(headPosition);
                    }
                    command.Extrude = false;
                }
                else
                {
                    for (int i = 0; i < 10; ++i)
                    {
                        FossFab.HeadPosition headPosition = new FossFab.HeadPosition();
                        headPosition.Pos = new FossFab.Vector3();
                        headPosition.Pos.X = i;
                        headPosition.Pos.Y = i;
                        headPosition.Pos.Z = i;
                        command.Positions.Add(headPosition);
                    }
                    command.Extrude = true;
                }
            }

            if (false)
            {

                command.CommandCode = FossFab.Command.Types.CommandCode.Move;
                FossFab.HeadPosition headPosition = new FossFab.HeadPosition();
                headPosition.Pos = new FossFab.Vector3();


                headPosition.Pos.X = position.x;
                headPosition.Pos.Y = position.y;
                headPosition.Pos.Z = position.z;

                command.Positions.Add(headPosition);

                command.Extrude = true;

            }
            if (false)
            {
                command.CommandCode = FossFab.Command.Types.CommandCode.RenderMesh;
                //make two triangles
                command.Mesh = new FossFab.Mesh();


                FossFab.Triangle triangle0 = new FossFab.Triangle();
                triangle0.Vertex1 = new FossFab.Vector3();
                triangle0.Vertex1.X = 0.0f;
                triangle0.Vertex1.Y = 0.0f;
                triangle0.Vertex1.Z = 0.0f;

                triangle0.Vertex2 = new FossFab.Vector3();
                triangle0.Vertex2.X = 1.0f;
                triangle0.Vertex2.Y = 0.0f;
                triangle0.Vertex2.Z = 0.0f;

                triangle0.Vertex3 = new FossFab.Vector3();
                triangle0.Vertex3.X = 0.0f;
                triangle0.Vertex3.Y = 2.0f;
                triangle0.Vertex3.Z = 0.0f;



                FossFab.Triangle triangle1 = new FossFab.Triangle();
                triangle1.Vertex1 = new FossFab.Vector3();
                triangle1.Vertex1.X = -1.0f;
                triangle1.Vertex1.Y = 2.0f;
                triangle1.Vertex1.Z = 3.0f;

                triangle1.Vertex2 = new FossFab.Vector3();
                triangle1.Vertex2.X = -10.0f;
                triangle1.Vertex2.Y = 2.0f;
                triangle1.Vertex2.Z = 3.0f;

                triangle1.Vertex3 = new FossFab.Vector3();
                triangle1.Vertex3.X = -10.0f;
                triangle1.Vertex3.Y = 25.0f;
                triangle1.Vertex3.Z = 35.0f;



                command.Mesh.Triangles.Add(triangle0);
                command.Mesh.Triangles.Add(triangle1);


            }

            if (time % 100 == 0)
            {
                //command.CommandCode = FossFab.Command.Types.CommandCode.Reset;
            


                byte[] msg = command.ToByteArray();

                int bytesSen1 = s.Send(msg);
            }


        }
    }

    void OnApplicationQuit()
    {
        client.Close();
        clientConnected = false;
    }

    void OnDisable()
    {
        client.Close();
        clientConnected = false;
    }
}