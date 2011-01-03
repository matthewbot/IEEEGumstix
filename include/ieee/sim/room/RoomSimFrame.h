#ifndef ROOMSIMFRAME_H
#define ROOMSIMFRAME_H

#include <wx/wx.h>
#include "ieee/sim/room/RoomSimSettingsDialog.h"
#include "ieee/sim/room/RobotPanelLayer.h"
#include "ieee/sim/shared/WorldPanel.h"
#include "ieee/sim/shared/GridPanelLayer.h"
#include "ieee/sim/shared/ObjectPanelLayer.h"

namespace ieee {
	class RoomSimFrame : public wxFrame, ObjectPanelLayer::Callbacks {
		public:
			RoomSimFrame();

			// WorldPanel::Callbacks
			virtual bool onWorldClicked(const Coord &pos);
			virtual void onWorldDragged(const Coord &pos);

		private:
			struct Settings : RoomSimSettingsDialog::Settings {
				Settings();
			};

			struct RoomPlannerConfig : RoomPlanner::Config {
				RoomPlannerConfig(const Settings &settings);
			};

			struct SimWorld : World {
				SimWorld(const Settings &settings, const RoomPlannerConfig &config);
			};

			RoomSimSettingsDialog roomsimsettingsdialog;
			Settings roomsimsettings;

			RoomPlannerConfig roomplannerconfig;

			SimWorld world;
			Robot robot;

			GridPanelLayer worldgridlayer;
			GridPanelLayer mapgridlayer;
			ObjectPanelLayer objectlayer;
			RobotPanelLayer robotlayer;
			WorldPanel worldpanel;

			World::iterator selectedobj;
			int selectedid;

			wxPanel buttonpanel;
			wxButton stepbutton;
			wxButton resetbutton;

			void onStepPressed(wxCommandEvent &event);
			void onResetPressed(wxCommandEvent &event);
			void onMenuOpen(wxCommandEvent &event);
			void onMenuSave(wxCommandEvent &event);
			void onMenuQuit(wxCommandEvent &event);
			void onMenuSettings(wxCommandEvent &event);
			void onMenuObjects(wxCommandEvent &event);
			void onMenuWorldGrid(wxCommandEvent &event);
			void onMenuMapGrid(wxCommandEvent &event);

			void showGrid(bool mapgrid);

			DECLARE_EVENT_TABLE()
	};
}

#endif
