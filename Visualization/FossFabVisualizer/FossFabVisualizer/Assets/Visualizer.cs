using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System.Linq;
using System;
using System.Text;


public class Visualizer : MonoBehaviour
{

    void OnGUI()
    {
        if (GUILayout.Button("Zero Camera"))
        {
           // Debug.Log(Camera.current);
            Camera.main.transform.localPosition = new Vector3(0.0f, 0.3f, -0.75f);
            Camera.main.transform.localRotation = Quaternion.Euler(0.0f, 0.0f, 0.0f);
        }

    }


    private bool printHeadInitialized;
    private Texture backgroundTexture;
    private Texture floorTexture;
    private string printedSegmentName = "PrintedSegment";

    // Use this for initialization

    //TcpListener listener;

    private Socket client = null;

    private int i = 0;
    const int byteCount = 1024;



    bool newData = false;


    List<GameObject> gameObjects;
    List<GameObject> printedSegments;

    GameObject printHead;
    GameObject targetMesh;


    private byte[] bytes;



    Socket listener;
    Socket handler;


    System.Threading.Thread SocketThread;
    volatile bool keepReading = false;


    //keep track of stuff to send:

    Queue<KeyValuePair<Vector3, bool>> targets;

    //for rendering
    Vector3 y_diff;

    float head_scale = 0.001f;

    void networkCode()
    {
        string data;

        // Data buffer for incoming data.
        bytes = new Byte[byteCount];

        // host running the application.

        IPAddress ipAddress = IPAddress.Parse("127.0.0.1");
        IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 11000);

        // Create a TCP/IP socket.
        listener = new Socket(ipAddress.AddressFamily,
            SocketType.Stream, ProtocolType.Tcp);

        // Bind the socket to the local endpoint and 
        // listen for incoming connections.

        try
        {
            listener.Bind(localEndPoint);
            listener.Listen(10);

            // Start listening for connections.
            while (true)
            {
                keepReading = true;

                // Program is suspended while waiting for an incoming connection.
                Debug.Log("Waiting for Connection");     //It works

                handler = listener.Accept();
                Debug.Log("Client Connected");     //It doesn't work
                data = null;

                // An incoming connection needs to be processed.
                while (keepReading)
                {
                    byte[] localBytes = new byte[1024];
                    int bytesRec = handler.Receive(localBytes);
                    // Debug.Log(bytesRec);
                    if (bytesRec > 0)
                    {
                        bytes = localBytes.Take(bytesRec).ToArray();
                        newData = true;
                    }


                    Debug.Log("Received from Server");

                    if (bytesRec <= 0)
                    {
                        keepReading = false;
                        handler.Disconnect(true);
                        break;
                    }

                    data += Encoding.ASCII.GetString(bytes, 0, bytesRec);
                    if (data.IndexOf("<EOF>") > -1)
                    {
                        break;
                    }

                    System.Threading.Thread.Sleep(1);

                }
                System.Threading.Thread.Sleep(1);
            }
        }
        catch (Exception e)
        {
            Debug.Log(e.ToString());
        }
    }



    void startServer()
    {
        SocketThread = new System.Threading.Thread(networkCode);
        SocketThread.IsBackground = true;
        SocketThread.Start();
    }







    void Start()
    {

        //logical setup:
        this.targets = new Queue<KeyValuePair<Vector3, bool>>();
        this.gameObjects = new List<GameObject>();
        this.printedSegments = new List<GameObject>();




        this.targetMesh = new GameObject("Target Mesh");
        this.targetMesh.AddComponent<MeshFilter>();
        this.targetMesh.AddComponent<MeshRenderer>();
        this.targetMesh.transform.parent = this.gameObject.transform;
        targetMesh.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        targetMesh.GetComponent<MeshRenderer>().material.color = Color.white;



        //Axes //TODO: replace these 
        /*
        GameObject xAxis = PrimitiveGenerator(PrimitiveType.Cylinder);
        GameObject yAxis = PrimitiveGenerator(PrimitiveType.Cylinder);
        GameObject zAxis = PrimitiveGenerator(PrimitiveType.Cylinder);
        xAxis.GetComponent<MeshRenderer>().material.color = new Color(1.0f, 0.0f, 0.0f);
        yAxis.GetComponent<MeshRenderer>().material.color = new Color(0.0f, 1.0f, 0.0f);
        zAxis.GetComponent<MeshRenderer>().material.color = new Color(0.0f, 0.0f, 1.0f);
        xAxis.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        yAxis.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        zAxis.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");

        xAxis.transform.localRotation = Quaternion.FromToRotation(Vector3.up, Vector3.right);
        zAxis.transform.localRotation = Quaternion.FromToRotation(Vector3.up, Vector3.forward);
        */

        //TODO: massively refactor this code


        //buildplate
        GameObject buildPlate = PrimitiveGenerator(PrimitiveType.Cube);
        //buildPlate.GetComponent<MeshRenderer>().material.color = new Color(0.2f, 0.2f, 0.2f);
        Texture checkerboardTexture = GameObject.Find("Textures").GetComponent<CheckerboardTexture>().MainTexture;
        buildPlate.GetComponent<MeshRenderer>().material.mainTexture = checkerboardTexture;
        buildPlate.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        
        buildPlate.transform.localScale = new Vector3(100.0f, 0.01f, 100.0f);

        /*
        //gantry
        GameObject backLeftColumn = PrimitiveGenerator(PrimitiveType.Cube);
        backLeftColumn.GetComponent<MeshRenderer>().material.color = new Color(0.2f, 0.2f, 0.2f);
        backLeftColumn.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        backLeftColumn.transform.localScale = new Vector3(0.01f, 0.3f, 0.01f);
        backLeftColumn.transform.localPosition = new Vector3(-buildPlate.transform.localScale.x / 2.0f, backLeftColumn.transform.localScale.y / 2.0f, -buildPlate.transform.localScale.z / 2.0f);

        GameObject frontLeftColumn = PrimitiveGenerator(PrimitiveType.Cube);
        frontLeftColumn.GetComponent<MeshRenderer>().material.color = new Color(0.2f, 0.2f, 0.2f);
        frontLeftColumn.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        frontLeftColumn.transform.localScale = new Vector3(0.01f, 0.3f, 0.01f);
        frontLeftColumn.transform.localPosition = new Vector3(-buildPlate.transform.localScale.x / 2.0f, frontLeftColumn.transform.localScale.y / 2.0f, buildPlate.transform.localScale.z / 2.0f);

        GameObject backRightColumn = PrimitiveGenerator(PrimitiveType.Cube);
        backRightColumn.GetComponent<MeshRenderer>().material.color = new Color(0.2f, 0.2f, 0.2f);
        backRightColumn.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        backRightColumn.transform.localScale = new Vector3(0.01f, 0.3f, 0.01f);
        backRightColumn.transform.localPosition = new Vector3(buildPlate.transform.localScale.x / 2.0f, backRightColumn.transform.localScale.y / 2.0f, -buildPlate.transform.localScale.z / 2.0f);

        GameObject frontRightColumn = PrimitiveGenerator(PrimitiveType.Cube);
        frontRightColumn.GetComponent<MeshRenderer>().material.color = new Color(0.2f, 0.2f, 0.2f);
        frontRightColumn.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        frontRightColumn.transform.localScale = new Vector3(0.01f, 0.3f, 0.01f);
        frontRightColumn.transform.localPosition = new Vector3(buildPlate.transform.localScale.x / 2.0f, frontRightColumn.transform.localScale.y / 2.0f, buildPlate.transform.localScale.z / 2.0f);
        */

        //background


        //backgroundTexture = Resources.Load<Texture2D>("Assets/Textures/metal") as Texture2D;
        GameObject background = PrimitiveGenerator(PrimitiveType.Cube);
        //background.GetComponent<MeshRenderer>().material.color = new Color(0.2f, 0.2f, 0.2f);
        background.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        background.transform.localScale = new Vector3(10000.0f, 10000.0f, 0.01f);
        background.transform.localPosition = new Vector3(0.0f, 0.0f, 100.0f);
        //background.GetComponent<MeshRenderer>().material.mainTexture = backgroundTexture;
        //background.GetComponent<MeshRenderer>().material.mainTextureScale = new Vector2(1000.0f, 1000.0f);
        background.GetComponent<MeshRenderer>().material.color = new Color(0.9f, 0.9f, 0.9f);


        //floor
        //floorTexture = Resources.Load<Texture2D>("Assets/Textures/wood");
        GameObject floor = PrimitiveGenerator(PrimitiveType.Cube);
        //background.GetComponent<MeshRenderer>().material.color = new Color(0.2f, 0.2f, 0.2f);
        floor.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        floor.transform.localScale = new Vector3(10000.0f, 0.0f, 10000.0f);
        floor.transform.localPosition = new Vector3(0.0f, -100.0f, 0.0f);
        //floor.GetComponent<MeshRenderer>().material.mainTexture = floorTexture;
        //floor.GetComponent<MeshRenderer>().material.mainTextureScale = new Vector2(0.01f, 0.01f);
        floor.GetComponent<MeshRenderer>().material.color = new Color(0.9f, 0.9f, 0.9f);

        //printhead

        /*
        printHead = PrimitiveGenerator(PrimitiveType.Cube);
        printHead.transform.localScale = new Vector3(0.1f, 0.1f, 0.1f);

        printHead.name = "PrintHead";
        printHead.transform.parent = this.gameObject.transform;
        printHead.transform.localPosition = backLeftColumn.transform.localPosition;
        */

        this.printHead = new GameObject("PrintHead");
        this.printHead.transform.parent = this.gameObject.transform;

        this.printHead.AddComponent<MeshRenderer>();
        this.printHead.AddComponent<MeshFilter>();

        this.printHead.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        this.printHead.GetComponent<MeshRenderer>().material.color = Color.green;


        Mesh holderMesh = new Mesh();
        ObjImporter newMesh = new ObjImporter();
        holderMesh = newMesh.ImportFile(Application.streamingAssetsPath + "/printhead.obj");
        //       "Assets/Meshes/printhead.obj");

        //Mesh holderMesh = printHead.GetComponent<MeshFilter>().mesh;
        Bounds bounds = holderMesh.bounds;


        MeshFilter filter = this.printHead.GetComponent<MeshFilter>();
        this.printHead.transform.localRotation = Quaternion.Euler(90.0f, 0.0f, 0.0f);
        filter.mesh = holderMesh;
        y_diff = new Vector3(0.0f, (bounds.max - bounds.min).z * head_scale, 0.0f) * 1.5f;



        this.printHead.transform.localScale = Vector3.one * head_scale;

        printHeadInitialized = false;




        Application.runInBackground = true;
        startServer();


    }

    //fossfab vector to Vector3
    Vector3 FFVToVector3(FossFab.Vector3 ffv)
    {
        return new Vector3(ffv.X, ffv.Y, ffv.Z);
    }

    private void Reset()
    {
        printHeadInitialized = false;
        printHead.transform.localPosition = Vector3.zero;
        targets.Clear();
        printedSegments.Clear();
        targetMesh.GetComponent<MeshFilter>().mesh.Clear();
        foreach (Transform child in this.gameObject.transform)
        {
            if (child.name == this.printedSegmentName)
            {
                GameObject.Destroy(child.gameObject);
            }
        }
    }

    // Update is called once per frame
    void Update()
    {



        if (newData)
        {


            FossFab.Command command = null;

            /*
            foreach (byte b in bytes)
            {
                Debug.Log(b);
            }
            */

                command = FossFab.Command.Parser.ParseFrom(bytes);


                FossFab.Command.Types.CommandCode command_code = command.CommandCode;

                Debug.Log(command.CalculateSize());

                switch (command_code)
                {

                    case FossFab.Command.Types.CommandCode.Reset:
                        Reset();
                        break;

                    case FossFab.Command.Types.CommandCode.Move:
                        this.printHead.GetComponent<MeshRenderer>().enabled = true;
                        foreach (FossFab.HeadPosition pos in command.Positions)
                        {
                            this.targets.Enqueue(new KeyValuePair<Vector3, bool>(FFVToVector3(pos.Pos), command.Extrude));
                        }


                        break;

                    case FossFab.Command.Types.CommandCode.RenderMesh:
                        this.printHead.GetComponent<MeshRenderer>().enabled = false;
                        Mesh mesh = targetMesh.GetComponent<MeshFilter>().mesh;
                        Debug.Log("Mesh is");
                        Debug.Log(mesh);

                        mesh.Clear();

                        // make changes to the Mesh by creating arrays which contain the new values
                        int num_triangles = command.Mesh.Triangles.Count;
                        int num_vertices = num_triangles * 3;

                        List<Vector3> vertices = new List<Vector3>();

                        mesh.vertices = new Vector3[num_vertices];
                        for (int i = 0; i < num_triangles; ++i)
                        {

                            FossFab.Vector3 vertex1 = command.Mesh.Triangles[i].Vertex1;
                            FossFab.Vector3 vertex2 = command.Mesh.Triangles[i].Vertex2;
                            FossFab.Vector3 vertex3 = command.Mesh.Triangles[i].Vertex3;


                            vertices.Add(FFVToVector3(vertex1));
                            vertices.Add(FFVToVector3(vertex2));
                            vertices.Add(FFVToVector3(vertex3));



                        }



                        mesh.triangles = Enumerable.Range(0, num_vertices).ToArray();
                        mesh.vertices = vertices.ToArray();


                        /*
                        mesh.vertices = new Vector3[] { new Vector3(0, 0, 0), new Vector3(0, 1, 0), new Vector3(1, 1, 0) };

                        mesh.triangles = new int[] { 0, 1, 2 };
                        */
                        mesh.RecalculateNormals();



                        break;

                    case FossFab.Command.Types.CommandCode.RenderPath:
                        this.printHead.GetComponent<MeshRenderer>().enabled = true;
                        foreach (FossFab.HeadPosition pos in command.Positions)
                        {
                            this.targets.Enqueue(new KeyValuePair<Vector3, bool>(FFVToVector3(pos.Pos), true));
                        }
                        break;

                    case FossFab.Command.Types.CommandCode.RenderSlices:

                        this.printHead.GetComponent<MeshRenderer>().enabled = false;
                        Reset();
                        //TODO: this is going to move the printhead.  Should fix this

                        Debug.Log("SLICE SLICE BABY");
                        foreach (FossFab.Slice slice in command.Slices)
                        {

                            foreach (FossFab.Vector3 pos in slice.Positions)
                            {
                                this.targets.Enqueue(new KeyValuePair<Vector3, bool>(FFVToVector3(pos), true));
                            }
                            this.targets.Enqueue(new KeyValuePair<Vector3, bool>(FFVToVector3(slice.Positions[0]), true));
                        }
                        break;


                    default:
                        break;
                }
            






            newData = false;
        }

        //TODO: Need to schedule and animate in here.     
        if (targets.Count > 0)
        {
            DrawHead(targets.Dequeue());
        }

    }


    private GameObject PrimitiveGenerator(PrimitiveType type)
    {
        GameObject temp = GameObject.CreatePrimitive(type);
        GameObject obj = GameObject.Instantiate(temp);
        obj.transform.parent = this.transform;
        Destroy(temp);
        return obj;
    }

    private GameObject GenerateSegment(Vector3 point0, Vector3 point1, string name, Color color)
    {
        float lineRadius = 0.002f;
        GameObject printedSegment = PrimitiveGenerator(PrimitiveType.Cylinder);
        printedSegment.GetComponent<MeshRenderer>().material.color = color;
        printedSegment.name = name;
        printedSegment.transform.parent = this.transform;
        float scaleX = lineRadius;
        float scaleZ = lineRadius;
        float scaleY = Vector3.Distance(point0, point1);

        printedSegment.transform.localScale = new Vector3(scaleX, scaleY * 0.5f, scaleZ);

        printedSegment.transform.localPosition = (point0 + point1) / 2.0f; //center between the two points

        //figure out the rotation

        Quaternion quat = Quaternion.FromToRotation(Vector3.up, point1 - point0);

        printedSegment.transform.localRotation = quat;

        return printedSegment;

    }

    private Color RandomColorGenerator()
    {
        return new Color(UnityEngine.Random.Range(0.0f, 1.0f), UnityEngine.Random.Range(0.0f, 1.0f), UnityEngine.Random.Range(0.0f, 1.0f));
    }

    private void DrawHead(KeyValuePair<Vector3, bool> drawpair)
    {
        Vector3 headPos = drawpair.Key + y_diff;

        bool extrude = drawpair.Value;

        Vector3 firstHeadPos = printHead.transform.localPosition;

        this.printHead.transform.localPosition = headPos;



        if (printHeadInitialized && extrude)
        {
            GameObject printedSegment = GenerateSegment(firstHeadPos - y_diff, headPos - y_diff, printedSegmentName, Color.cyan);

            this.printedSegments.Add(printedSegment);
        }
        else
        {
            printHeadInitialized = true;
        }


    }

    private void DrawJoint(Matrix4x4 transform)
    {

        GameObject joint = PrimitiveGenerator(PrimitiveType.Cylinder);

        joint.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        joint.GetComponent<MeshRenderer>().material.color = RandomColorGenerator();

        joint.transform.localPosition = new Vector3(transform.m03, transform.m23, transform.m13);
        Vector3 euler = transform.rotation.eulerAngles;
        joint.transform.localRotation = Quaternion.Euler(euler[0], euler[2], euler[1]);


        gameObjects.Add(joint);

    }


    private void DrawLink(Matrix4x4 transform, Vector3 extents)
    {
        GameObject link = PrimitiveGenerator(PrimitiveType.Cube);

        link.transform.localPosition = new Vector3(transform.m03, transform.m23, transform.m13); //switching directions again
        Vector3 euler = transform.rotation.eulerAngles;
        link.transform.localRotation = Quaternion.Euler(euler[0], euler[2], euler[1]);

        link.transform.localScale = extents;

        link.GetComponent<MeshRenderer>().material.shader = Shader.Find("Custom/StandardSurfaceShader");
        link.GetComponent<MeshRenderer>().material.color = RandomColorGenerator();

        gameObjects.Add(link);

    }


    void stopServer()
    {
        keepReading = false;

        //stop thread
        if (SocketThread != null)
        {
            SocketThread.Abort();
        }

        if (handler != null && handler.Connected)
        {
            handler.Disconnect(false);
            Debug.Log("Disconnected!");
        }
    }


    void OnDisable()
    {
        stopServer();
    }




}
