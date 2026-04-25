using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System;
using UnityEngine.Tilemaps;

public class CreateLevel : MonoBehaviour
{
    public GameObject player;                   // Reference to the player object.
                                                // We need to position it according to the level.
    public GameObject floor, wall_1, door, goal, bat;  // References to objects we need to instantiate to
                                                // build the level.

    // Start is called before the first frame update
    void Start()
    {
        // dataPath is the directory path to the Assets in the project
        // We want to load file map.txt inside directory Maps.
        string filename = Application.dataPath + "/Maps/testing_map.txt";

        if (File.Exists(filename))
        {
            // Read map file line by line tokenizing them into the numbers we need.
            // Specifically, width, height and the tile ids for all positions in the map.

            TextReader reader = File.OpenText(filename);
            string line = reader.ReadLine();
            string[] tokens = line.Split(' ');
            int width, height;
            width = int.Parse(tokens[0]);
            height = int.Parse(tokens[1]);

            // Decorative walls
            for (int y = 0; y < height; y++) // Left wall
            {
                GameObject obj = Instantiate(wall_1, new Vector3(-1f, 0.5f, y), transform.rotation);
                obj.transform.parent = transform;
                obj.transform.Rotate(0.0f, 90.0f, 0.0f);
            }
            
            for (int x = 0; x < width; x++) // Top wall
            {
                GameObject obj;
                if (x == width/2) obj = Instantiate(door, new Vector3(x, 0.5f, height), transform.rotation);
                else obj = Instantiate(wall_1, new Vector3(x, 0.5f, height), transform.rotation);
                obj.transform.parent = transform;
                obj.transform.Rotate(0.0f, 180.0f, 0.0f);  
            }

            // Other elements
            for (int y = height - 1; y >= 0; y--)
            {
                line = reader.ReadLine();
                tokens = line.Split(" ");
                for (int x = 0; x < width; x++)
                {
                    int tile = int.Parse(tokens[x]);

                    // All tiles will have a floor instance under them. We instantiate it here.
                    GameObject obj = Instantiate(floor, new Vector3(x, -0.75f, y), transform.rotation);
                    // All instances created by this script end as children of the object that contains the script.
                    obj.transform.parent = transform;

                    // Now, for objects other than the player we spawn an instance.
                    switch (tile)
                    {
                        case 6: // Player
                            player.transform.position = new Vector3(x, 0.0f, y);
                            break;
                        case 7: // Bat
                            obj = Instantiate(bat, new Vector3(x, 0.0f, y), transform.rotation);
                            obj.transform.parent = transform;
                            break;
                    }
                }
            }
        }
        else
        {
            // Hopefully this should not happen. But just in case ...
            Debug.Log("Map file could not be found!!!");
        }
    }
}
