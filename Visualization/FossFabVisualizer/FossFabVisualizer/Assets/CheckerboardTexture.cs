using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckerboardTexture : MonoBehaviour {

    private Texture2D mainTexture;

    public Texture2D MainTexture {
            get
            {
                return mainTexture;
            }
        }

    private int mainTexWidth;
    private int mainTexHeight;

    void Awake () {
        mainTexture = new Texture2D(mainTexWidth, mainTexHeight);
        mainTexHeight = 256;
        mainTexWidth = 256;

        SetMainTextureSize();
        CreatePattern();
    }

    private void CreatePattern()
    {
        for (int i = 0; i < mainTexWidth; i++)
        {
            for (int j = 0; j < mainTexWidth; j++)
            {
                if (((i + j) % 2) == 1)
                {
                    mainTexture.SetPixel(i, j, Color.black);
                }
                else
                {
                    mainTexture.SetPixel(i, j, Color.white);
                }

            }
        }

        mainTexture.Apply();
        mainTexture.wrapMode = TextureWrapMode.Clamp;
    }

    private void SetMainTextureSize()
    {
        mainTexture = new Texture2D(mainTexWidth, mainTexHeight);
        mainTexture.filterMode = FilterMode.Point;
    }

    // Update is called once per frame
    void Update () {
		
	}
}
