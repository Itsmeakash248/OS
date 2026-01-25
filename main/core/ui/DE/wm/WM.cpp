#include "WM.hpp"
#include "../DE.hpp"
#include "core/apps/AppManager.hpp"
#include "core/tasks/gui/GuiTask.hpp"
#include <algorithm>

WM &WM::getInstance() {
  static WM instance;
  return instance;
}

WM::WM()
    : m_windowContainer(nullptr), m_appContainer(nullptr), m_screen(nullptr),
      m_statusBar(nullptr), m_dock(nullptr), m_fullScreenWindow(nullptr) {}

WM::~WM() {}

void WM::init(lv_obj_t *window_container, lv_obj_t *app_container,
                        lv_obj_t *screen, lv_obj_t *status_bar, lv_obj_t *dock) {
  m_windowContainer = window_container;
  m_appContainer = app_container;
  m_screen = screen;
  m_statusBar = status_bar;
  m_dock = dock;
}

void WM::collect_windows(lv_obj_t *parent, std::vector<lv_obj_t *> &windows) {
  if (!parent)
    return;

  uint32_t cnt = lv_obj_get_child_count(parent);
  for (int i = cnt - 1; i >= 0; i--) {
    lv_obj_t *child = lv_obj_get_child(parent, i);

    if (lv_obj_get_user_data(child) != nullptr) {
      windows.push_back(child);
    } else {
      collect_windows(child, windows);
    }
  }
}

void WM::update_layout(lv_obj_t *root_container) {
  if (!root_container)
    return;

  GuiTask::lock();
  std::vector<lv_obj_t *> all_windows;
  collect_windows(root_container, all_windows);

  std::vector<lv_obj_t *> visible_windows;
  for (auto it = all_windows.rbegin(); it != all_windows.rend(); ++it) {
    if (!lv_obj_has_flag(*it, LV_OBJ_FLAG_HIDDEN)) {
      visible_windows.push_back(*it);
    } else {
      lv_obj_set_parent(*it, root_container);
      lv_obj_add_flag(*it, LV_OBJ_FLAG_HIDDEN);
    }
  }

  for (auto *win : visible_windows) {
    lv_obj_set_parent(win, root_container);
  }

  uint32_t child_cnt = lv_obj_get_child_count(root_container);
  for (int i = child_cnt - 1; i >= 0; i--) {
    lv_obj_t *child = lv_obj_get_child(root_container, i);
    if (lv_obj_get_user_data(child) == nullptr) {
      lv_obj_delete(child);
    }
  }

  lv_obj_t *current_parent = root_container;
  lv_obj_remove_flag(current_parent, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_flow(current_parent, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(current_parent, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_all(current_parent, 0, 0);
  lv_obj_set_style_pad_gap(current_parent, lv_dpx(4), 0);

  for (size_t i = 0; i < visible_windows.size(); ++i) {
    lv_obj_t *win = visible_windows[i];

    lv_obj_remove_flag(win, LV_OBJ_FLAG_FLOATING);
    lv_obj_set_size(win, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_grow(win, 1);
    lv_obj_set_style_margin_all(win, 0, 0);
    lv_obj_set_style_pad_all(win, 0, 0);

    if (i == visible_windows.size() - 1) {
      lv_obj_set_parent(win, current_parent);
    } else {
      lv_obj_set_parent(win, current_parent);

      lv_obj_t *next_container = lv_obj_create(current_parent);
      lv_obj_remove_style_all(next_container);
      lv_obj_remove_flag(next_container, LV_OBJ_FLAG_SCROLLABLE);
      lv_obj_set_flex_grow(next_container, 1);
      lv_obj_set_size(next_container, lv_pct(100), lv_pct(100));
      lv_obj_set_style_pad_gap(next_container, lv_dpx(4), 0);

      lv_flex_flow_t current_flow =
          lv_obj_get_style_flex_flow(current_parent, LV_PART_MAIN);
      lv_flex_flow_t next_flow = (current_flow == LV_FLEX_FLOW_ROW)
                                     ? LV_FLEX_FLOW_COLUMN
                                     : LV_FLEX_FLOW_ROW;
      lv_obj_set_flex_flow(next_container, next_flow);

      current_parent = next_container;
    }
  }
  GuiTask::unlock();
}

lv_obj_t *WM::get_layout_root(lv_obj_t *obj) {
  lv_obj_t *curr = obj;
  while (curr) {
    lv_obj_t *parent = lv_obj_get_parent(curr);
    if (parent && lv_obj_get_parent(parent) == nullptr) {
      return curr;
    }
    curr = parent;
  }
  return nullptr;
}

void WM::activate_window(lv_obj_t *target_win) {
  GuiTask::lock();
  lv_obj_t *root = get_layout_root(target_win);
  if (!root) {
    GuiTask::unlock();
    return;
  }

  std::vector<lv_obj_t *> windows;
  collect_windows(root, windows);

  for (auto *win : windows) {
    lv_obj_t *dock_btn = (lv_obj_t *)lv_obj_get_user_data(win);
    bool is_active = (win == target_win);

    if (is_active)
      lv_obj_add_state(win, LV_STATE_FOCUSED);
    else
      lv_obj_remove_state(win, LV_STATE_FOCUSED);

    lv_obj_set_style_border_width(win, lv_dpx(is_active ? 2 : 1), 0);
    lv_obj_set_style_border_opa(win, is_active ? LV_OPA_COVER : LV_OPA_40, 0);

    if (dock_btn) {
      if (is_active)
        lv_obj_add_state(dock_btn, LV_STATE_CHECKED);
      else
        lv_obj_remove_state(dock_btn, LV_STATE_CHECKED);
    }
    lv_obj_invalidate(win);
  }
  GuiTask::unlock();
}

void WM::openApp(const std::string &packageName) {
  GuiTask::lock();
  for (const auto &pair : m_windowAppMap) {
    if (pair.second == packageName) {
      lv_obj_t *win = pair.first;

      if (lv_obj_has_flag(win, LV_OBJ_FLAG_HIDDEN)) {
        lv_obj_clear_flag(win, LV_OBJ_FLAG_HIDDEN);

        lv_obj_t *dock_btn = (lv_obj_t *)lv_obj_get_user_data(win);
        if (dock_btn) {
          lv_obj_add_state(dock_btn, LV_STATE_CHECKED);
        }

        lv_obj_t *root = get_layout_root(win);
        if (root)
          update_layout(root);
      }

      activate_window(win);
      System::Apps::AppManager::getInstance().startApp(packageName);
      GuiTask::unlock();
      return;
    }
  }

  auto app =
      System::Apps::AppManager::getInstance().getAppByPackageName(packageName);
  if (!app) {
    GuiTask::unlock();
    return;
  }

  lv_obj_t *win = lv_win_create(m_windowContainer);
  lv_obj_set_size(win, lv_pct(95), lv_pct(95));
  lv_obj_set_style_radius(win, lv_dpx(8), 0);
  lv_obj_set_style_border_post(win, true, 0);
  lv_obj_center(win);
  lv_obj_add_flag(win, LV_OBJ_FLAG_FLOATING);

  m_windowAppMap[win] = packageName;

  const char *iconSymbol = (const char *)app->getIcon();

  lv_obj_t *dock_btn = DE::create_dock_btn(m_appContainer, iconSymbol, lv_pct(15), lv_pct(85));
  lv_obj_set_style_bg_opa(dock_btn, LV_OPA_TRANSP, 0);
  lv_obj_set_style_bg_opa(dock_btn, LV_OPA_80, LV_STATE_CHECKED);
  lv_obj_add_state(dock_btn, LV_STATE_CHECKED);
  lv_obj_add_event_cb(dock_btn, on_win_minimize, LV_EVENT_CLICKED, this);

  lv_obj_set_user_data(win, dock_btn);
  lv_obj_set_user_data(dock_btn, win);

  lv_obj_t *header = lv_win_get_header(win);
  lv_obj_set_height(header, lv_pct(10));
  lv_obj_set_style_pad_all(header, 0, 0);
  lv_obj_add_flag(header, LV_OBJ_FLAG_EVENT_BUBBLE);
  lv_obj_remove_flag(header, LV_OBJ_FLAG_SCROLLABLE);
  lv_win_add_title(win, app->getAppName().c_str());

  lv_obj_t *min_btn = lv_win_add_button(win, LV_SYMBOL_DOWN, lv_pct(10));
  lv_obj_add_event_cb(min_btn, on_header_minimize, LV_EVENT_CLICKED, this);

  lv_obj_t *max_btn = lv_win_add_button(win, LV_SYMBOL_PLUS, lv_pct(10));
  if (lv_obj_get_child_count(max_btn) > 0) {
    m_windowMaxBtnLabelMap[win] = lv_obj_get_child(max_btn, 0);
  }
  lv_obj_add_event_cb(max_btn, on_win_maximize, LV_EVENT_CLICKED, this);

  lv_obj_t *close_btn = lv_win_add_button(win, LV_SYMBOL_CLOSE, lv_pct(10));
  lv_obj_add_event_cb(close_btn, on_win_close, LV_EVENT_CLICKED, this);

  lv_obj_t *content = lv_win_get_content(win);
  lv_obj_set_style_pad_all(content, 0, 0);
  lv_obj_add_flag(content, LV_OBJ_FLAG_EVENT_BUBBLE);

  app->createUI(content);

  lv_obj_add_event_cb(win, on_win_focus, LV_EVENT_PRESSED, this);
  activate_window(win);

  update_layout(m_windowContainer);

  System::Apps::AppManager::getInstance().startApp(app);
  GuiTask::unlock();
}

void WM::closeApp(const std::string &packageName) {
  lv_obj_t *winToClose = nullptr;
  for (const auto &pair : m_windowAppMap) {
    if (pair.second == packageName) {
      winToClose = pair.first;
      break;
    }
  }

  if (winToClose) {
    closeWindow(winToClose);
  }
}

void WM::on_win_focus(lv_event_t *e) {
  WM *wm = (WM *)lv_event_get_user_data(e);
  lv_obj_t *win = lv_event_get_current_target_obj(e);
  activate_window(win);

  if (wm && wm->m_windowAppMap.count(win)) {
    System::Apps::AppManager::getInstance().startApp(wm->m_windowAppMap[win]);
  }
}

void WM::on_win_minimize(lv_event_t *e) {
  WM *wm = (WM *)lv_event_get_user_data(e);
  lv_obj_t *db = lv_event_get_target_obj(e);
  lv_obj_t *w = (lv_obj_t *)lv_obj_get_user_data(db);
  if (w) {
    if (w == wm->m_fullScreenWindow) {
      wm->toggleFullScreen(w);
    }
    bool is_hidden = lv_obj_has_flag(w, LV_OBJ_FLAG_HIDDEN);
    bool is_active =
        (lv_obj_get_style_border_width(w, LV_PART_MAIN) == lv_dpx(2));

    if (is_hidden) {
      lv_obj_clear_flag(w, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_state(db, LV_STATE_CHECKED);
      activate_window(w);
      if (wm && wm->m_windowAppMap.count(w)) {
        System::Apps::AppManager::getInstance().startApp(wm->m_windowAppMap[w]);
      }
    } else {
      if (!is_active) {
        activate_window(w);
        if (wm && wm->m_windowAppMap.count(w)) {
          System::Apps::AppManager::getInstance().startApp(
              wm->m_windowAppMap[w]);
        }
      } else {
        lv_obj_add_flag(w, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_state(db, LV_STATE_CHECKED);
      }
    }
    lv_obj_t *root = get_layout_root(w);
    if (root)
      update_layout(root);
  }
}

void WM::on_header_minimize(lv_event_t *e) {
  WM *wm = (WM *)lv_event_get_user_data(e);
  lv_obj_t *btn = lv_event_get_target_obj(e);
  lv_obj_t *header = lv_obj_get_parent(btn);
  lv_obj_t *w = lv_obj_get_parent(header);

  if (wm && w) {
    if (w == wm->m_fullScreenWindow) {
      wm->toggleFullScreen(w);
    }
    lv_obj_t *dock_btn = (lv_obj_t *)lv_obj_get_user_data(w);
    lv_obj_add_flag(w, LV_OBJ_FLAG_HIDDEN);
    if (dock_btn) {
      lv_obj_remove_state(dock_btn, LV_STATE_CHECKED);
    }
    lv_obj_t *root = get_layout_root(w);
    if (root)
      update_layout(root);
  }
}

void WM::closeWindow(lv_obj_t *w) {
  if (!w)
    return;

  GuiTask::lock();
  if (w == m_fullScreenWindow) {
    toggleFullScreen(w);
  }

  if (m_windowAppMap.count(w)) {
    std::string pkg = m_windowAppMap[w];
    m_windowAppMap.erase(w);
    System::Apps::AppManager::getInstance().stopApp(pkg);
  }

  if (m_windowMaxBtnLabelMap.count(w)) {
    m_windowMaxBtnLabelMap.erase(w);
  }

  lv_obj_t *db = (lv_obj_t *)lv_obj_get_user_data(w);
  lv_obj_t *root = get_layout_root(w);
  if (db)
    lv_obj_delete(db);
  lv_obj_delete(w);
  if (root)
    update_layout(root);
  GuiTask::unlock();
}

void WM::on_win_close(lv_event_t *e) {
  WM *wm = (WM *)lv_event_get_user_data(e);
  lv_obj_t *btn = lv_event_get_target_obj(e);

  lv_obj_t *header = lv_obj_get_parent(btn);
  lv_obj_t *w = lv_obj_get_parent(header);

  if (wm)
    wm->closeWindow(w);
}

void WM::on_win_maximize(lv_event_t *e) {
  WM *wm = (WM *)lv_event_get_user_data(e);
  lv_obj_t *btn = lv_event_get_target_obj(e);
  lv_obj_t *header = lv_obj_get_parent(btn);
  lv_obj_t *w = lv_obj_get_parent(header);

  if (wm)
    wm->toggleFullScreen(w);
}

void WM::toggleFullScreen(lv_obj_t *win) {
  GuiTask::lock();
  lv_obj_t *max_btn_content = nullptr;
  if (m_windowMaxBtnLabelMap.count(win)) {
    max_btn_content = m_windowMaxBtnLabelMap[win];
  }

  if (m_fullScreenWindow == win) {
    lv_obj_set_parent(win, m_windowContainer);
    lv_obj_remove_flag(win, LV_OBJ_FLAG_FLOATING);
    lv_obj_clear_flag(m_statusBar, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(m_dock, LV_OBJ_FLAG_HIDDEN);
    m_fullScreenWindow = nullptr;
    update_layout(m_windowContainer);
    if (max_btn_content) {
      lv_image_set_src(max_btn_content, LV_SYMBOL_PLUS);
    }
  } else {
    if (m_fullScreenWindow) {
      toggleFullScreen(m_fullScreenWindow);
    }

    lv_obj_set_parent(win, m_screen);
    lv_obj_set_size(win, lv_pct(100), lv_pct(100));
    lv_obj_add_flag(win, LV_OBJ_FLAG_FLOATING);
    lv_obj_add_flag(m_statusBar, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(m_dock, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(win);
    m_fullScreenWindow = win;
    if (max_btn_content) {
      lv_image_set_src(max_btn_content, LV_SYMBOL_MINUS);
    }
  }
  GuiTask::unlock();
}