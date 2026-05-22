using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System;
using UnityEngine.Tilemaps;

public class CreateLevel : MonoBehaviour
{
    public GameObject player;
    public GameObject floor, wall_1, door, bat, spikeTrap, arrowTrap, axeTrap, slime, barrel, zombie, coin, cauldron, rock, candle, shelf, weaponRack, witch;

    public static int[,] mapLayout;
    public static int mapWidth, mapHeight;

    [Header("Level Palette")]
    public LevelPalette[] levelPalettes;
    private LevelPalette levelPalette;
    [HideInInspector] public DoorController currentDoor;


    // Spawn simple
    private GameObject Spawn(GameObject prefab, Vector3 pos, Quaternion rot, Transform parent)
    {
        GameObject obj = Instantiate(prefab, pos, rot);
        obj.transform.parent = parent;
        return obj;
    }

    // Spawn de objetos con color de paleta
    private GameObject SpawnWithPalette(GameObject prefab, Vector3 pos, Quaternion rot, Transform parent, Color color)
    {
        GameObject obj = Spawn(prefab, pos, rot, parent);
        ApplyPalette(obj, color);
        return obj;
    }

    // Spawn de objetos
    private GameObject SpawnAndRegister(GameObject prefab, Vector3 pos, Quaternion rot, Transform parent, Vector2Int gridPos)
    {
        GameObject obj = Spawn(prefab, pos, rot, parent);
        OccupancyManager.Register(gridPos, obj);
        return obj;
    }

    // Spawn de enemigos
    private void SpawnEnemy(GameObject prefab, Vector3 pos, Vector2Int gridPos)
    {
        GameObject obj = Spawn(prefab, pos, transform.rotation, transform);
        OccupancyManager.Register(gridPos, obj);
        FindObjectOfType<LevelManager>().RegisterEnemy();
    }

    // Devuelve el color de suelo de la paleta activa, o blanco si no hay paleta.
    private Color FloorColor => levelPalette != null ? levelPalette.floorColor  : Color.white;
    private Color WallColor => levelPalette != null ? levelPalette.wallColor   : Color.white;

    
    // Paletas de colores
    private void ApplyPalette(GameObject obj, Color targetColor)
    {
        Renderer objRenderer = obj.GetComponentInChildren<Renderer>();
        if (objRenderer != null)
            objRenderer.material.color = targetColor;
    }

    private void ApplyLevelPalette()
    {
        Camera mainCam = Camera.main;
        if (mainCam != null)
        {
            mainCam.clearFlags = CameraClearFlags.SolidColor;
            mainCam.backgroundColor = levelPalette.skyboxColor;
        }

        GameObject fogPlane = GameObject.Find("FogPlane");
        if (fogPlane != null)
        {
            Renderer fogRenderer = fogPlane.GetComponent<Renderer>();
            if (fogRenderer != null)
                fogRenderer.material.SetColor("_Color", levelPalette.fogColor);
        }

        RenderSettings.ambientMode = UnityEngine.Rendering.AmbientMode.Flat;
        RenderSettings.ambientLight = levelPalette.ambientLightColor;
    }

    // Generador de rotas flotantes
    private void GenerateFloatingRocks(int width, int height)
    {
        int rockCount = UnityEngine.Random.Range(3, 15);
        int margin = 15;
        Camera mainCam = Camera.main;

        if (mainCam == null) { Debug.LogError("Main Camera not found!"); return; }

        int attempts = 0, spawnedRocks = 0;

        while (spawnedRocks < rockCount && attempts < 50)
        {
            attempts++;

            int targetX = UnityEngine.Random.Range(-margin, width  + margin);
            int targetY = UnityEngine.Random.Range(-margin, height + margin);

            // Descartar posiciones dentro del mapa
            if (targetX >= 0 && targetX < width && targetY >= 0 && targetY < height) continue;

            Vector3 rockPosition = new Vector3(targetX, -2.75f, targetY);

            // Descartar posiciones fuera de la cámara
            Vector3 viewportPos = mainCam.WorldToViewportPoint(rockPosition);
            if (!(viewportPos.x >= 0f && viewportPos.x <= 1f &&
                viewportPos.y >= 0f && viewportPos.y <= 1f &&
                viewportPos.z > 0f)) continue;

            Quaternion rot   = Quaternion.Euler(
                UnityEngine.Random.Range(0, 4) * 90f,
                UnityEngine.Random.Range(0, 4) * 90f,
                UnityEngine.Random.Range(0, 4) * 90f);
            float scale = UnityEngine.Random.Range(0.5f, 1.4f);

            GameObject floatingRock = Spawn(rock, rockPosition, rot, transform);
            floatingRock.transform.localScale = Vector3.one * scale;
            spawnedRocks++;
        }
    }

    // Spawn de suelos inferiores
    private void SpawnSupportFloors(int x, int y, Transform parent)
    {
        for (int i = 1; i <= 3; i++)
            SpawnWithPalette(floor, new Vector3(x, -0.75f - i, y), transform.rotation, parent, FloorColor);
    }

    // Spawn de objetos, entidades...
    private void SpawnTileObject(int tile, int x, int y, Transform parent)
    {
        Vector3 pos = new Vector3(x, 0f, y);
        Vector2Int gPos = new Vector2Int(x, y);
        float rotMult;

        switch (tile)
        {
            case 2:  SpawnAndRegister(cauldron, pos, transform.rotation, parent, gPos); break;
            case 3:  SpawnAndRegister(candle, pos, transform.rotation, parent, gPos); break;
            case 4:  SpawnAndRegister(shelf, pos, Quaternion.Euler(0,  90, 0), parent, gPos); break;
            case 5:  SpawnAndRegister(shelf, pos, Quaternion.Euler(0, 180, 0), parent, gPos); break;
            case 6:  SpawnPlayer(pos); break;
            case 7:  SpawnEnemy(bat, pos, gPos); break;
            case 9:  SpawnEnemy(slime, pos, gPos); break;
            case 10: SpawnAndRegister(barrel, pos, transform.rotation, parent, gPos); break;
            case 11: SpawnEnemy(zombie, pos, gPos); break;
            case 12: SpawnEnemy(witch, pos, gPos); break;

            case 13: case 14: case 15: case 16:
                rotMult = tile - 13;
                GameObject arrowObj = SpawnWithPalette(arrowTrap, pos, transform.rotation, parent, FloorColor);
                arrowObj.transform.Rotate(0f, 90f * rotMult, 0f);
                OccupancyManager.Register(gPos, arrowObj);
                break;

            case 17: case 18: case 19: case 20:
                rotMult = tile - 17;
                GameObject axeObj = SpawnWithPalette(axeTrap, pos, transform.rotation, parent, FloorColor);
                axeObj.transform.Rotate(0f, 90f * rotMult, 0f);
                OccupancyManager.Register(gPos, axeObj);
                break;

            case 21: Spawn(coin, pos, transform.rotation, parent); break;
            case 22: SpawnAndRegister(weaponRack, pos, transform.rotation,        parent, gPos); break;
            case 23: SpawnAndRegister(weaponRack, pos, Quaternion.Euler(0, 90, 0), parent, gPos); break;
        }
    }

    private void SpawnPlayer(Vector3 pos)
    {
        PlayerController pc = player.GetComponent<PlayerController>();
        if (pc != null) pc.TeleportEntity(pos);
        else player.transform.position = pos;
    }

    private void SpawnDecorativeWalls(int width, int height, GameObject[] rowParents)
    {
        // Pared izquierda
        for (int y = 0; y < height; y++)
        {
            if (mapLayout[0, y] == 0) continue;
            GameObject obj = SpawnWithPalette(wall_1, new Vector3(-1f, 0.5f, y), transform.rotation, rowParents[y].transform, WallColor);
            obj.transform.Rotate(0f, 90f, 0f);
        }

        // Pared superior + puerta
        for (int x = 0; x < width; x++)
        {
            if (mapLayout[x, height - 1] == 0) continue;
            GameObject obj;
            if (x == width / 2)
            {
                obj = SpawnWithPalette(door, new Vector3(x, 0.5f, height), transform.rotation, rowParents[height - 1].transform, WallColor);
                currentDoor = obj.GetComponent<DoorController>();
            }
            else
            {
                obj = SpawnWithPalette(wall_1, new Vector3(x, 0.5f, height), transform.rotation, rowParents[height - 1].transform, WallColor);
            }
            obj.transform.Rotate(0f, 180f, 0f);
        }

        // Suelo de la puerta
        SpawnWithPalette(floor, new Vector3(width / 2, -0.75f, height), transform.rotation, rowParents[height - 1].transform, FloorColor);

        // Cerrar puerta si hay enemigos
        if (LevelManager.instance != null && LevelManager.instance.totalEnemies > 0 && currentDoor != null)
            currentDoor.Close();    
    }

    public void GenerateLevel(int levelNumber)
    {
        ClearLevel();
        OccupancyManager.ClearAll();

        string filename = Application.dataPath + "/Maps/level_" + levelNumber + ".txt";

        if (!File.Exists(filename))
        {
            Debug.Log("Map file could not be found!!!");
            return;
        }

        TextReader reader = File.OpenText(filename);
        string[] tokens = reader.ReadLine().Split(' ');
        int width = int.Parse(tokens[0]);
        int height = int.Parse(tokens[1]);

        mapLayout = new int[width, height];
        mapWidth = width;
        mapHeight = height;

        // Paleta
        levelPalette = levelPalettes[UnityEngine.Random.Range(0, levelPalettes.Length)];
        if (levelPalette != null) ApplyLevelPalette();

        // Padres de fila
        GameObject[] rowParents = new GameObject[height];
        for (int y = 0; y < height; y++)
        {
            rowParents[y] = new GameObject("Row_" + y);
            rowParents[y].transform.parent = transform;
        }

        // Tiles
        for (int y = height - 1; y >= 0; y--)
        {
            tokens = reader.ReadLine().Split(' ');
            for (int x = 0; x < width; x++)
            {
                int tile = int.Parse(tokens[x]);
                mapLayout[x, y] = tile;

                if (tile == 0) continue;

                SpawnSupportFloors(x, y, rowParents[y].transform);

                if (tile == 8)
                {
                    SpawnWithPalette(spikeTrap, new Vector3(x, -0.75f, y), transform.rotation, rowParents[y].transform, FloorColor);
                }
                else
                {
                    SpawnWithPalette(floor, new Vector3(x, -0.75f, y), transform.rotation, rowParents[y].transform, FloorColor);
                    SpawnTileObject(tile, x, y, rowParents[y].transform);
                }
            }
        }

        SpawnDecorativeWalls(width, height, rowParents);
        GenerateFloatingRocks(width, height);
    }

    private void ClearLevel()
    {
        foreach (Transform child in transform)
            Destroy(child.gameObject);
    }
}