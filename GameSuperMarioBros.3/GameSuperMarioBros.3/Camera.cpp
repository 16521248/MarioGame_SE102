#include "Camera.h"
#include "Map.h"
#include "Mario.h"
#include "define.h"
#include "Utils.h"

Camera* Camera::__instance = NULL;

Camera::Camera()
{
	DebugOut(L"INFO Camera::");

	cameraPosition = D3DXVECTOR3(0, 0, 0);
	Setwidthheight(CAMERA_WIDTH, CAMERA_HEIGHT);
}

Camera* Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

D3DXVECTOR3 Camera::GetPositionInCamera(D3DXVECTOR3 position)
{
	D3DXVECTOR3 pos;
	pos.x = position.x - this->cameraPosition.x;
	pos.y = position.y - this->cameraPosition.y;
	return D3DXVECTOR3(pos.x, pos.y, 0);
}

void Camera::SetCameraPosition(float x, float y)
{
	if (!block)
	{
		this->cameraPosition.x = x;
		this->cameraPosition.y = y;
	}
}

void Camera::Setwidthheight(int x, int y)
{
	this->width = x;
	this->height = y;
}

std::vector<LPGAMEOBJECT> Camera::GetlistinCamera(vector<LPGAMEOBJECT> obj)
{
	vector<LPGAMEOBJECT> list;
	list.clear();
	float x = cameraPosition.x;
	float y = cameraPosition.y;

	for (int i = 0; i < obj.size(); i++)
	{
		if (obj.at(i)->x > x && obj.at(i)->x <x + width && obj.at(i)->y >y && obj.at(i)->y < y + height)
			list.push_back(obj.at(i));
	}
	return list;
}

D3DXVECTOR3 Camera::GetCameraPosition()
{
	return this->cameraPosition;
}

void Camera::Update(DWORD dt, int typeCamera, float& countx)
{
	// Update camera to follow mario
	float cx, cy, mapheight, mapwidth;

	mapheight = Map::GetInstance()->GetHeight();
	mapwidth = Map::GetInstance()->GetWidth();
	DebugOut(L"mapheight = %d ", mapheight);
	CMario* player = CMario::GetInstance();

	player->GetPosition(cx, cy);
	if (player->iswag)
	{
		if (player->nx > 0)
			cx -= 8;
		else if (player->nx < 0)
			cx += 8;
	}
	D3DXVECTOR3 pos = GetCameraPosition();

	if (typeCamera)
	{
		if (pos.x > mapwidth - SCREEN_WIDTH - 1)
			return;
		countx = countx + CAMERA_SPEED_X*dt;
		cx = countx;
		cy = 0;
		SetCameraPosition((int)countx, 0);
		return;
	}
	else
	{
		if (mapwidth > SCREEN_WIDTH) {
			if (cx + 5 < SCREEN_WIDTH / 2) {
				cx = pos.x;
			}
			else if (cx + SCREEN_WIDTH / 2 > mapwidth - 1) {
				cx = mapwidth - SCREEN_WIDTH;
			}
			else {
				cx = cx + 5 + SCREEN_WIDTH / 2 - SCREEN_WIDTH;
			}
		}
		else {
			cx = 0;
		}

		if (mapheight > SCREEN_HEIGHT)
		{
			if (cy < SCREEN_HEIGHT - 32)
			{
				cy = 0;
			}
			else if (cy > mapheight - SCREEN_HEIGHT)
			{
				cy = mapheight - SCREEN_HEIGHT + 32;
			}
			else //if (cy < mapheight - SCREEN_HEIGHT)
			{
				cy = cy - SCREEN_HEIGHT / 2 + 32;
			}
		}
		else
		{
			cy = mapheight - SCREEN_HEIGHT;
		}
		if (cy < 0) cy = 0;
		//cy -= SCREEN_HEIGHT / 2;
	}
	SetCameraPosition((int)cx, (int)cy);
}

void Camera::Unload()
{
	if (__instance == NULL) return;
	else
	{
		__instance = NULL;
	}
}

Camera::~Camera()
{

}




