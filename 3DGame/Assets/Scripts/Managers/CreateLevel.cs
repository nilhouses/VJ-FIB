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
    public GameObject floor, wall_1, door, bat, spikeTrap, slime, barrel, zombie, witch;  // References to objects we need to instantiate to
                                                // build the level.
    public static int[,] mapLayout;
    public static int mapWidth, mapHeight;

    // Start is called before the first frame update
    public void GenerateLevel(int levelNumber)
    {
        ClearLevel();                   // Limpiar el nivel actual antes de generar uno nuevo
        OccupancyManager.ClearAll();    // Limpiar el OccupancyManager para el nuevo nivel

        // dataPath is the directory path to the Assets in the project
        // We want to load file map.txt inside directory Maps.
        string filename = Application.dataPath + "/Maps/level_" + levelNumber + ".txt";

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

            // Inicializamos la matriz lógica
            mapLayout = new int[width, height];
            mapWidth = width;
            mapHeight = height;

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

            // Door floor
            GameObject doorFloor = Instantiate(floor, new Vector3(width/2, -0.75f, height), transform.rotation);
            doorFloor.transform.parent = transform;

            // Other elements
            for (int y = height - 1; y >= 0; y--)
            {
                line = reader.ReadLine();
                tokens = line.Split(" ");
                for (int x = 0; x < width; x++)
                {
                    int tile = int.Parse(tokens[x]);
                    mapLayout[x, y] = tile; // Guardamos el tile en la matriz lógica

                    Vector3 floorPosition = new Vector3(x, -0.75f, y); // La posición del nivel del suelo

                    // Si el tile es un spike trap, no colocamos el suelo normal, sino directamente el spike trap
                    if (tile == 8) 
                    {
                        GameObject obj = Instantiate(spikeTrap, floorPosition, transform.rotation);
                        obj.transform.parent = transform;
                    }
                    else    // Si el tile no es un spike trap, colocamos el suelo normal y luego comprobamos si hay algo encima 
                    {
                        // Ponemos el suelo normal primero
                        GameObject floorObj = Instantiate(floor, floorPosition, transform.rotation);
                        floorObj.transform.parent = transform;

                        // Y ahora comprobamos si hay algo encima del suelo
                        switch (tile)
                        {
                            case 6: // Player
                                Vector3 startPos = new Vector3(x, 0.0f, y);
                                PlayerController pc = player.GetComponent<PlayerController>();
                                if (pc != null) {
                                    pc.TeleportEntity(startPos);
                                } else {
                                    player.transform.position = startPos;
                                }
                                break;
                            case 7: // Bat
                                GameObject batObj = Instantiate(bat, new Vector3(x, 0.0f, y), transform.rotation);
                                batObj.transform.parent = transform;
                                // Registramos al enemigo en el LevelManager
                                FindObjectOfType<LevelManager>().RegisterEnemy();
                                // Registramos al enemigo en el OccupancyManager
                                Vector2Int batPos = new Vector2Int(x, y);
                                OccupancyManager.Register(batPos, batObj);
                                break;
                            case 9: // Slime
                                GameObject slimeObj = Instantiate(slime, new Vector3(x, 0.0f, y), transform.rotation);
                                slimeObj.transform.parent = transform;
                                // Registramos al enemigo en el LevelManager
                                FindObjectOfType<LevelManager>().RegisterEnemy();
                                // Registramos al enemigo en el OccupancyManager
                                Vector2Int slimePos = new Vector2Int(x, y);
                                OccupancyManager.Register(slimePos, slimeObj);
                                break;
                            case 10: // Barrel
                                GameObject barrelObj = Instantiate(barrel, new Vector3(x, 0.0f, y), transform.rotation);
                                barrelObj.transform.parent = transform;
                                break;
                            case 11: // Zombie
                                GameObject zombieObj = Instantiate(zombie, new Vector3(x, 0.0f, y), transform.rotation);
                                zombieObj.transform.parent = transform;
                                // Registramos al enemigo en el LevelManager
                                FindObjectOfType<LevelManager>().RegisterEnemy();
                                // Registramos al enemigo en el OccupancyManager
                                Vector2Int zombiePos = new Vector2Int(x, y);
                                OccupancyManager.Register(zombiePos, zombieObj);
                                break;
                            // (monedas, enemigos, etc.)
                            case 12: // Witch
                                GameObject witchObj = Instantiate(witch, new Vector3(x, 0.0f, y), transform.rotation);
                                witchObj.transform.parent = transform;
                                // Registramos al enemigo en el LevelManager
                                FindObjectOfType<LevelManager>().RegisterEnemy();
                                // Registramos al enemigo en el OccupancyManager
                                Vector2Int witchPos = new Vector2Int(x, y);
                                OccupancyManager.Register(witchPos, witchObj);
                                break;
                        }
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

    // Método para limpiar el nivel actual destruyendo todos los objetos hijos del nivel
    private void ClearLevel()
    {
        // Destruir todos los objetos hijos del nivel para limpiar el nivel actual
        foreach (Transform child in transform)
        {
            Destroy(child.gameObject);
        }
    }
}
