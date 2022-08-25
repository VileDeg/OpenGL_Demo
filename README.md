![Crave Logo](https://i.ibb.co/1vsnKJ4/crave-engine-logo.png)
# Crave Engine
***Crave*** is a primitive game engine developed with **OpenGL** for the sake of learning.
It is in early stage of development and currently only available for **Windows**.

## Building the engine
Clone the repository via
```sh
git clone https://github.com/VileDeg/crave-engine
```
***Crave*** uses [premake5](https://premake.github.io/) build system so first to *configure* run **GenerateProjects.bat** file.
By default it's contents should look like this:
```bat
call vendor\premake5.exe vs2022
PAUSE
```
You can change **vs2022** option, for available options see [premake documentation](https://premake.github.io/).
After projects were configured, build the solution through visual studio(if configured with **vs****** option).

The solution consists of **Crave** static library and **CavernEditor** console application that uses the library.
In premake5.lua script there is a postbuild task to call the scripts that will copy all necessary DLLs into executable folder.
If, for some reason, you get a _"missing DLL"_ error, consider copying necessary DLLs yourself. DLLs can be found at following paths:
* Crave/vendor/assimp/lib/
## Engine features
***Crave Engine*** is currently in early stage of development so it isn't yet possible to create games with it, but here is what it currently allows:
### Navigation
_Unity-like_ camera controls: 
* **Hold RMB** to rotate view
* **Hold MMB** to pan view
* **Scroll mouse wheel** to move forward and backward

![Navigation-encoded](https://user-images.githubusercontent.com/84181987/186784278-daf84815-be03-4207-a0e9-5c05a6667a4b.gif)
### Select and transform objects
Select _objects(entities)_ with **LMB**. Use **Q**, **W**, **E**, **R** to enable _translation_, _scale_ and _rotation_ gizmos.

![Transformation](https://user-images.githubusercontent.com/84181987/186784318-7615c9f4-7aa4-4912-8ffe-dc87566dbb0c.gif)
### Object hierarchy
**Drag and drop** one object on another to make it a child

![Hierarchy](https://user-images.githubusercontent.com/84181987/186784343-60d94465-23a9-4d6d-a733-b18b30cd7853.gif)
### Importing models
Currently ***Crave*** only supports importing models with _diffuse_ or _diffuse and specular_ textures.
### Save/load scene
Currently(_after last modifications_) saving and loading scene does nothing, although the buttons can be found in **File menu**.
## Attribution
Some textures you'll find under _CavernEditor_ resource folder belong to [Joey de Vries](https://twitter.com/JoeyDeVriez), specifically to [learnopengl](https://learnopengl.com/) course.
You can find any other licenses in _Third-party_licenses_ folder under _vendor_ directory.
