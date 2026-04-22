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
    public GameObject ground, wall, box, goal;  // References to objects we need to instantiate to
                                                // build the level.

    // Start is called before the first frame update
    void Start()
    {
        // dataPath is the directory path to the Assets in the project
        // We want to load file map.txt inside directory Maps.
        string filename = Application.dataPath + "/Maps/map.txt";

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
            for (int y = 0; y < height; y++)
            {
                line = reader.ReadLine();
                tokens = line.Split(" ");
                for (int x = 0; x < width; x++)
                {
                    int tile = int.Parse(tokens[x]);

                    // All tiles will have a ground instance under them. We instantiate it here.
                    GameObject obj = Instantiate(ground, new Vector3(x, 0.0f, y), transform.rotation);
                    // All instances created by this script end as children of the object that contains the script.
                    obj.transform.parent = transform;

                    // Now, for objects other than the player we spawn an instance.
                    switch (tile)
                    {
                        case 2:
                            obj = Instantiate(wall, new Vector3(x, 0.0f, y), transform.rotation);
                            obj.transform.parent = transform;
                            break;
                        case 3:
                            obj = Instantiate(goal, new Vector3(x, 0.0f, y), transform.rotation);
                            obj.transform.parent = transform;
                            break;
                        case 5:
                            obj = Instantiate(box, new Vector3(x, 0.0f, y), transform.rotation);
                            obj.transform.parent = transform;
                            break;
                        case 6:
                            // For the player, we position it at the location of the tile with the player tile id.
                            player.transform.Translate(x, 0.0f, y);
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
