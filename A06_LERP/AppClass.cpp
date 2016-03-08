#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

	lerpVectors = new vector3[numVectors];
	lerpVectors[0] = vector3(-4, -2, 5);
	lerpVectors[1] = vector3(1, -2, 5);
	lerpVectors[2] = vector3(-3, -1, 3);
	lerpVectors[3] = vector3(2, -1, 3);
	lerpVectors[4] = vector3(-2, 0, 0);
	lerpVectors[5] = vector3(3, 0, 0);
	lerpVectors[6] = vector3(-1, 1, -3);
	lerpVectors[7] = vector3(4, 1, -3);
	lerpVectors[8] = vector3(0, 2, -5);
	lerpVectors[9] = vector3(5, 2, -5);
	lerpVectors[10] = vector3(1, 3, -5);

	wayPoints = new PrimitiveClass[numVectors];

	for (int i = 0; i < numVectors; i++)
	{
		wayPoints[i] = PrimitiveClass();
		wayPoints[i].GenerateSphere(.1f, 3, RERED);
	}

	m_m4Model = IDENTITY_M4;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	animationTime += fTimeSpan;
	m_m4Model = glm::translate(IDENTITY_M4,
						glm::lerp(lerpVectors[currentWayPoint % numVectors],
							lerpVectors[(currentWayPoint + 1) % numVectors], float(animationTime / fDuration))
						);
	if (animationTime > fDuration) {
		animationTime = 0;
		currentWayPoint += 1;
		currentWayPoint %= numVectors;
	}
	m_pMeshMngr->SetModelMatrix(m_m4Model, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	for (int i = 0; i < numVectors; i++)
	{
		wayPoints[i].Render(m_pCameraMngr->GetProjectionMatrix(),
			m_pCameraMngr->GetViewMatrix(),
			glm::translate(IDENTITY_M4, lerpVectors[i]));
	}

	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	delete[] lerpVectors;
	delete[] wayPoints;
	super::Release(); //release the memory of the inherited fields
}