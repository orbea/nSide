#include "../nSide.hpp"

#include "system-properties.cpp"
unique_pointer<SystemProperties> systemProperties;

#include "systems.cpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "hotkeys.cpp"
#include "advanced.cpp"
unique_pointer<SettingsManager> settingsManager;

SettingsManager::SettingsManager() {
  settingsManager = this;

  layout.setMargin(5);
  statusBar.setFont(Font().setBold());

  refreshLocale();
  setSize({600, 405});
  setAlignment({0.0, 1.0});
  setDismissable();

  onSize([&] {
    input.mappingList.resizeColumns();
    hotkeys.mappingList.resizeColumns();
  });
}

auto SettingsManager::refreshLocale() -> void {
  setTitle(locale["Settings"]);
  systems.refreshLocale();
  video.refreshLocale();
  audio.refreshLocale();
  input.refreshLocale();
  hotkeys.refreshLocale();
  advanced.refreshLocale();

  setSize(geometry().size());
}

auto SettingsManager::setVisible(bool visible) -> SettingsManager& {
  if(visible) {
    input.refreshMappings();
    hotkeys.refreshMappings();
  }
  Window::setVisible(visible);
  return *this;
}

auto SettingsManager::show(uint setting) -> void {
  panel.item(setting)->setSelected();
  setVisible();
  setFocused();
  doSize();
}
