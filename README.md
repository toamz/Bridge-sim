**Bridge-sim** is a simple bridge simulation app made for windows using SFML.

## Controls

Middle click to run
Right click a node to select node
Right click empty space to deselect
Right drag with nothing selected to move hovered node
Left click empy space to create a new node connected with selected node
Left click selected node to make it fixed

Last created node can be deleted when selected by right clicking it, when trying to delete other node, the selection is switched to the one which can be deleted

Move the window when the simulation is running to interact with gravity!

## How to build

Make sure SFML is installed and integrated with Visual Studio using vcpkg with following commands:

```
vcpkg integrate install
vcpkg install sfml
```

Open solution in Visual Studio and build!

## Short demonstration

[Bridge-sim demonstration - YouTube](https://youtu.be/y1xBMRpTKfg)
