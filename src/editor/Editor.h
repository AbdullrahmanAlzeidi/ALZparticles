#pragma once

#include "environment/EnvironmentData.h"
#include "misc/StartMenu.h"

namespace Ainan {
	bool SaveEnvironment(const EnvironmentData& env, std::string path);
	std::string CheckEnvironmentFile(const std::string& path);
	EnvironmentData* LoadEnvironment(const std::string& path);

	class Editor
	{
		enum EditorStatus
		{
			Status_EditorMode,
			Status_PlayMode,
			Status_PauseMode,
			Status_ExportMode
		};

		enum class Profiler
		{
			ParticleProfiler,
			PlaymodeProfiler,
			RenderingProfiler
		};

	public:
		Editor();
		~Editor();

		void Update();
		void Draw();
		void StartFrame();
		void EndFrame();

	public:
		bool ShouldDelete = false;

	private:
		EnvironmentData* m_Env = nullptr;
		StartMenu m_StartMenu;
		Camera m_Camera;
		ViewportWindow m_ViewportWindow;
		AppStatusWindow m_AppStatusWindow;
		ExportCamera m_ExportCamera;
		RenderSurface m_RenderSurface;
		Gizmo m_Gizmo;
		Grid m_Grid;

		bool m_EnvironmentControlsWindowOpen = true;
		bool m_ObjectInspectorWindowOpen = true;
		bool m_ProfilerWindowOpen = true;
		EditorStatus m_Status = Status_EditorMode;
		std::string m_EnvironmentFolderPath;
		std::shared_ptr<Texture> m_PlayButtonTexture;
		std::shared_ptr<Texture> m_PauseButtonTexture;
		std::shared_ptr<Texture> m_StopButtonTexture;
		float m_TimeSincePlayModeStarted = 0.0f;
		bool m_ExportedFrame = false;
		bool m_AddObjectWindowOpen = false;
		std::string m_AddObjectWindowObjectName = "Example Name";
		EnvironmentObjectType m_AddObjectWindowObjectType = SpriteType;
		std::clock_t m_TimeStart = 0, m_TimeEnd = 0;
		float m_DeltaTime = 0.0f;
		std::array<float, 120> m_DeltaTimeHistory;
		Profiler m_ActiveProfiler = Profiler::RenderingProfiler;

	private:
		void DisplayMainMenuBarGUI();
		void DisplayEnvironmentControlsGUI();
		void Stop();
		void Pause();
		void Resume();
		void ExportMode();
		void PlayMode();
		void DisplayObjectInspecterGUI();
		void RefreshObjectOrdering();
		void Duplicate(EnvironmentObjectInterface& obj);
		void FocusCameraOnObject(EnvironmentObjectInterface& object);
		void AddEnvironmentObject(EnvironmentObjectType type, const std::string& name);
		void RegisterEnvironmentInputKeys();
		void DisplayProfilerGUI();
		void UpdateTitle();
	};
}

