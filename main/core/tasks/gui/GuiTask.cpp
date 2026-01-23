#include "GuiTask.hpp"
#include "LovyanGFX.hpp"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lvgl.h"
#include "src/drivers/display/lovyan_gfx/lv_lovyan_gfx.h"
#include "core/system/SystemManager.hpp"
#include "core/apps/AppManager.hpp"
#include "core/ui/DE/DE.hpp"
#include "core/ui/theming/ThemeEngine.hpp"

static const char *TAG = "GuiTask";
SemaphoreHandle_t GuiTask::xGuiSemaphore = nullptr;

GuiTask::GuiTask() : System::Task("gui_task", 10 * 1024, 5, 1) {
  if (!xGuiSemaphore) xGuiSemaphore = xSemaphoreCreateRecursiveMutex();
}

void GuiTask::lock() { if (xGuiSemaphore) xSemaphoreTakeRecursive(xGuiSemaphore, portMAX_DELAY); }
void GuiTask::unlock() { if (xGuiSemaphore) xSemaphoreGiveRecursive(xGuiSemaphore); }

void GuiTask::display_init() {
  lv_init();
  lv_fs_stdio_init();
  const uint32_t sz = 240 * 320 / 10 * 2;
  void *buf = heap_caps_malloc(sz, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
  if (!buf) buf = heap_caps_malloc(sz, MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM);
  
  lv_display_t *disp = lv_lovyan_gfx_create(240, 320, buf, sz, true);
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);
  
  lv_group_t * g = lv_group_create();
  lv_group_set_default(g);
  
  lv_tick_set_cb([]() { return (uint32_t)(esp_timer_get_time() / 1000); });
}

void GuiTask::run(void *) {
  lock();
  display_init();
  ThemeEngine::init();
  System::SystemManager::getInstance().initGuiState();
  DE::getInstance().init();
  unlock();

  setWatchdogTimeout(5000);

  while (true) {
    heartbeat();
    lock();
    uint32_t delay = lv_timer_handler();
    unlock();
    vTaskDelay(pdMS_TO_TICKS(delay));
  }
}