# VJ-FIB

## Info
* **Obrir VS des de l'explorador de fitxers a partir de la solution del projecte :** `.sln`.
* **Executar:** `Ctrl + F5` (Build + Run).
* **Cal fer retarget** Si no fas servir VS2015, **Botó dret al Projecte (01-window) > Retarget Projects**. Si no, no compilarà (error de SDK). Si fas el retarget al fitxer de configuració es passa de `<PlatformToolset>v140</PlatformToolset>` a `<PlatformToolset>v143</PlatformToolset>` (VS2015 o VS2022).

## Fitxers
* **.sln** Contenidor pare que agrupa tots els projectes i configuracions.
* **.vcxproj:** És la configuració de cada projecte (com un Makefile). Diu quins .cpp usar i quines llibreries carregar.
* **Prohibit:** No pugis mai la carpeta `Debug/` ni `Release/`.