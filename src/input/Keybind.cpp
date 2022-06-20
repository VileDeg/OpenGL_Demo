#include "Keybind.h"

void Keybind_Forward::OnPress(Camera cam) const
{
	cam.MoveForward();
}

void Keybind_Backward::OnPress(Camera cam) const
{
	cam.MoveBackward();
}

void Keybind_Left::OnPress(Camera cam) const
{
	cam.MoveLeft();
}

void Keybind_Right::OnPress(Camera cam) const
{
	cam.MoveRight();
}
