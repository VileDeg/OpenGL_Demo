![This is an image](https://i.ibb.co/1vsnKJ4/crave-engine-logo.png)

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
In premake5.lua script there is a postbuild task to call thr scripts that will copy all necessary DLLs into executable folder.
If for some reason you get a "missing DLL" error, consider copying necessary DLLs yourself. DLLs can be found at following paths:
* Crave/vendor/assimp/lib/
## Engine features
***Crave Engine*** is currently in early stage of development so it isn't yet possible to create games with it, but here is what it currently allows:
### Navigation
Unity-like camera controls: 
* Hold RMB to rotate view
* Hold MMB to pan view
* Scroll mouse wheel to move forward and backward

![Navigation-encoded](https://user-images.githubusercontent.com/84181987/186469220-6ddd605f-2ed7-4cb7-bce7-f4cc4bbd187f.gif)


## Attribution
Some textures you'll find under _CavernEditor_ resource folder belong to [Joey de Vries](https://twitter.com/JoeyDeVriez), specifically to [learnopengl](https://learnopengl.com/) course.
You can find any other licenses in _Third-party\_licenses_ folder under _vendor_ directory
