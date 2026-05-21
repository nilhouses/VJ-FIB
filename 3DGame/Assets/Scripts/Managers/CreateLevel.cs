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
    public GameObject floor, wall_1, door, bat, spikeTrap, arrowTrap, axeTrap, slime, barrel, zombie, witch, coin, cauldron, rock;  // References to objects we need to instantiate to
                                                // build the level.
    public static int[,] mapLayout;
    public static int mapWidth, mapHeight;
    
    [Header("Level Palette")]
    public LevelPalette[] levelPalettes;   // Array of level palettes to choose from
    private LevelPalette levelPalette;     // The palette for the current level
    [HideInInspector] public DoorController currentDoor;
    
    private void ApplyPalette(GameObject obj, Color targetColor)
    {
        if (levelPalette != null)
        {
            // Buscamos en el objeto o en sus hijos
            Renderer objRenderer = obj.GetComponentInChildren<Renderer>();
            if (objRenderer != null)
            {
                objRenderer.material.color = targetColor;
            }
        }
    }

    private void GenerateFloatingRocks(int width, int height)
    {
        int rockCount = UnityEngine.Random.Range(3, 15);

        int margin = 15;
        int minX = -margin;
        int maxX = width + margin;
        int minY = -margin;
        int maxY = height + margin;

        Camera mainCam = Camera.main;

        if (mainCam == null)
        {
            Debug.LogError("Main Camera not found!");
            return;
        }

        int attempts = 0;
        int spawnedRocks = 0;

        while (spawnedRocks < rockCount && attempts < 50)
        {
            attempts++;

            // Coordenada random
            int targetX = UnityEngine.Random.Range(minX, maxX);
            int targetY = UnityEngine.Random.Range(minY, maxY);

            // Eliminar rocas que estén dentro del mapa
            if (targetX >= 0 && targetX < width && targetY >= 0 && targetY < height)
            {
                continue; 
            }

            // 2 Niveles por debajo del suelo (-0.75f - 2.0f = -2.75f)
            Vector3 rockPosition = new Vector3(targetX, -2.75f, targetY);

            // Eliminar rocas que estén fuera de la cámara
            Vector3 viewportPos = mainCam.WorldToViewportPoint(rockPosition);
            if (!(viewportPos.x >= 0f && viewportPos.x <= 1f && viewportPos.y >= 0f && viewportPos.y <= 1f && viewportPos.z > 0f))
            {
                continue; 
            }

            // Rotación y escalado random
            float rotX = UnityEngine.Random.Range(0, 4) * 90f; //0.0f alternativamente
            float rotY = UnityEngine.Random.Range(0, 4) * 90f;
            float rotZ = UnityEngine.Random.Range(0, 4) * 90f; //0.0f alternativamente
            Quaternion gridRotation = Quaternion.Euler(rotX, rotY, rotZ);
            
            // Para que  sean algo distintas
            float randomScale = UnityEngine.Random.Range(0.5f, 1.4f);

            // Crear la roca que esté fuera el rango
            GameObject floatingRock = Instantiate(rock, rockPosition, gridRotation);
            floatingRock.transform.parent = transform;
            floatingRock.transform.localScale = new Vector3(randomScale, randomScale, randomScale);

            spawnedRocks++;
        }
    }

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

            // Seleccionar la paleta para el nivel actual
            int randomIndex = UnityEngine.Random.Range(0, levelPalettes.Length);
            levelPalette = levelPalettes[randomIndex];

            if (levelPalette != null)
            {
                // Configurar la cámara para que el fondo sea el color de la niebla
                Camera mainCam = Camera.main;
                if (mainCam != null)
                {
                    mainCam.clearFlags = CameraClearFlags.SolidColor;
                    mainCam.backgroundColor = levelPalette.skyboxColor;
                }

                // Configurar el color de la niebla
                GameObject fogPlane = GameObject.Find("FogPlane");
                if (fogPlane != null)
                {
                    Renderer fogRenderer = fogPlane.GetComponent<Renderer>();
                    if (fogRenderer != null)
                    {
                        fogRenderer.material.SetColor("_Color", levelPalette.fogColor);
                    }
                }

                // Luz ambiental
                RenderSettings.ambientMode = UnityEngine.Rendering.AmbientMode.Flat;
                RenderSettings.ambientLight = levelPalette.ambientLightColor;
            }
            
            float rotationMultiplier; // Variable auxiliar para calcular la rotación de las trampas de flechas y hachas

            // Creamos los padres para las filas
            GameObject[] rowParents = new GameObject[height];
            for (int y = 0; y < height; y++)
            {
                rowParents[y] = new GameObject("Row_" + y);
                rowParents[y].transform.parent = transform;
            }

            for (int y = height - 1; y >= 0; y--)
            {
                line = reader.ReadLine();
                tokens = line.Split(" ");
                for (int x = 0; x < width; x++)
                {
                    int tile = int.Parse(tokens[x]);
                    mapLayout[x, y] = tile; // Guardamos el tile en la matriz lógica

                    if (tile == 0) continue; // Si el tile es 0, no colocamos nada (ni suelo ni nada encima)

                    Vector3 floorPosition = new Vector3(x, -0.75f, y); // La posición del nivel del suelo

                    // Suelos inferiores
                    for (int i = 1; i <= 3; i++)
                    {
                        GameObject supportFloor = Instantiate(floor, new Vector3(x, -0.75f - i, y), transform.rotation);
                        supportFloor.transform.parent = rowParents[y].transform;
                        ApplyPalette(supportFloor, levelPalette != null ? levelPalette.floorColor : Color.white);
                    }

                    // Si el tile es un spike trap, no colocamos el suelo normal, sino directamente el spike trap
                    if (tile == 8) 
                    {
                        GameObject obj = Instantiate(spikeTrap, floorPosition, transform.rotation);
                        obj.transform.parent = rowParents[y].transform;
                        ApplyPalette(obj, levelPalette != null ? levelPalette.floorColor : Color.white);
                    }
                    else    // Si el tile no es un spike trap, colocamos el suelo normal y luego comprobamos si hay algo encima 
                    {
                        // Ponemos el suelo normal primero
                        GameObject floorObj = Instantiate(floor, floorPosition, transform.rotation);
                        floorObj.transform.parent = rowParents[y].transform;
                        ApplyPalette(floorObj, levelPalette != null ? levelPalette.floorColor : Color.white);

                        // Y ahora comprobamos si hay algo encima del suelo
                        switch (tile)
                        {
                            case 2: // Cauldron
                                GameObject cauldronObj = Instantiate(cauldron, new Vector3(x, 0.0f, y), transform.rotation);
                                cauldronObj.transform.parent = rowParents[y].transform;
                                Vector2Int cauldronPos = new Vector2Int(x, y);
                                OccupancyManager.Register(cauldronPos, cauldronObj);
                                break;
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
                                barrelObj.transform.parent = rowParents[y].transform;
                                Vector2Int barrelPos = new Vector2Int(x, y);
                                OccupancyManager.Register(barrelPos, barrelObj);
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
                            case 12: // Witch
                                GameObject witchObj = Instantiate(witch, new Vector3(x, 0.0f, y), transform.rotation);
                                witchObj.transform.parent = transform;
                                // Registramos al enemigo en el LevelManager
                                FindObjectOfType<LevelManager>().RegisterEnemy();
                                // Registramos al enemigo en el OccupancyManager
                                Vector2Int witchPos = new Vector2Int(x, y);
                                OccupancyManager.Register(witchPos, witchObj);
                                break;
                            case 13: // Arrow Trap Shoot Up
                            case 14: // Arrow Trap Shoot Right
                            case 15: // Arrow Trap Shoot Down
                            case 16: // Arrow Trap Shoot Left
                                GameObject arrowTrapObj = Instantiate(arrowTrap, new Vector3(x, 0.0f, y), transform.rotation);
                                arrowTrapObj.transform.parent = rowParents[y].transform;
                                // Rotacion
                                rotationMultiplier = (float)(tile - 13);
                                arrowTrapObj.transform.Rotate(0f, 90f * rotationMultiplier, 0f);
                                ApplyPalette(arrowTrapObj, levelPalette != null ? levelPalette.floorColor : Color.white);
                                // Registramos la trampa en el OccupancyManager
                                Vector2Int arrowPos = new Vector2Int(x, y);
                                OccupancyManager.Register(arrowPos, arrowTrapObj);
                                break;
                            case 17: // Axe Trap Hit Up
                            case 18: // Axe Trap Hit Right
                            case 19: // Axe Trap Hit Down
                            case 20: // Axe Trap Hit Left
                                GameObject axeObj = Instantiate(axeTrap, new Vector3(x, 0.0f, y), transform.rotation);
                                axeObj.transform.parent = rowParents[y].transform;
                                // Rotacion
                                rotationMultiplier = (float)(tile - 17);
                                axeObj.transform.Rotate(0f, 90f * rotationMultiplier, 0f);
                                ApplyPalette(axeObj, levelPalette != null ? levelPalette.floorColor : Color.white);
                                // Registramos la trampa en el OccupancyManager
                                Vector2Int axePos = new Vector2Int(x, y);
                                OccupancyManager.Register(axePos, axeObj);
                                break;
                            case 21: // Coins
                                GameObject coinsObj = Instantiate(coin, new Vector3(x, 0.0f, y), transform.rotation);
                                coinsObj.transform.parent = rowParents[y].transform;
                                break;
                        }
                    }
                }
            }

            // Decorative walls
            for (int y = 0; y < height; y++) // Left wall
            {
                if (mapLayout[0, y] == 0) continue;
                GameObject obj = Instantiate(wall_1, new Vector3(-1f, 0.5f, y), transform.rotation);
                obj.transform.parent = rowParents[y].transform;
                obj.transform.Rotate(0.0f, 90.0f, 0.0f);
                ApplyPalette(obj, levelPalette != null ? levelPalette.wallColor : Color.white);
            }
            
            for (int x = 0; x < width; x++) // Top wall
            {
                if (mapLayout[x, height - 1] == 0) continue;
                GameObject obj;
                if (x == width/2) {
                    obj = Instantiate(door, new Vector3(x, 0.5f, height), transform.rotation);
                    currentDoor = obj.GetComponent<DoorController>();
                }
                else obj = Instantiate(wall_1, new Vector3(x, 0.5f, height), transform.rotation);
                obj.transform.parent = rowParents[height - 1].transform;
                obj.transform.Rotate(0.0f, 180.0f, 0.0f);
                ApplyPalette(obj, levelPalette != null ? levelPalette.wallColor : Color.white);
            }

            // Door floor
            GameObject doorFloor = Instantiate(floor, new Vector3(width/2, -0.75f, height), transform.rotation);
            doorFloor.transform.parent = rowParents[height - 1].transform;
            ApplyPalette(doorFloor, levelPalette != null ? levelPalette.floorColor : Color.white);

            // Add Floating Rocks
            GenerateFloatingRocks(width, height);
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
