
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include "StatusLine.h"
#include "TextPane.h"
#include "ToggleListPane.h"

#include "PacketListPane.h"

enum FocusState {
  PANE1 = 1,
  PANE2,
  PANE3,
};

class TuiFrontend {
  FocusState fstate;
 public:
  PacketListPane *pane1;
  ToggleListPane *pane2;
  TextPane       *pane3;
  Statusline     *sline;

 public:
  TuiFrontend();
  void init();
  void refresh();
  virtual ~TuiFrontend();
  void key_input(char c);
  void focuse_sw();
  FocusState fstate_get() const { return fstate; }
};


void TuiFrontend::focuse_sw()
{
  switch (fstate) {
    case PANE1: fstate = PANE2; break;
    case PANE2: fstate = PANE3; break;
    case PANE3: fstate = PANE1; break;
    default: assert(false);
  }
}


/*
 * Function Implementation
 */
TuiFrontend::TuiFrontend()
  : fstate(PANE1)
  , pane1(nullptr)
  , pane2(nullptr)
  , pane3(nullptr)
  , sline(nullptr) {}
TuiFrontend::~TuiFrontend()
{
  if (pane1) delete pane1;
  if (pane2) delete pane2;
  if (pane3) delete pane3;
  if (sline) delete sline;
}
void TuiFrontend::init()
{
  initscr();
  noecho();
  scrollok(stdscr, false);

  size_t sublines = LINES/3-1;
  pane1 = new PacketListPane(0, sublines*0+1, COLS, sublines-1);
  pane2 = new ToggleListPane(0, sublines*1  , COLS, sublines  );
  pane3 = new TextPane      (0, sublines*2  , COLS, sublines  );
  sline = new Statusline    (0, sublines*3+1, COLS);

  pane1->init(stdscr);
  pane2->init(stdscr);
  pane3->init(stdscr);
  sline->init(stdscr);
}
void TuiFrontend::refresh()
{
  if (pane1) pane1->refresh();
  if (pane2) pane2->refresh();
  if (pane3) pane3->refresh();
  if (sline) sline->refresh();
}
void TuiFrontend::key_input(char c)
{
  char inc = ' ';

  if (c == '\t') {
    focuse_sw();
  } else {
    switch (fstate) {
      case PANE1:
        pane1->key_input(c);
        break;
      case PANE2:
        pane2->key_input(c);
        break;
      case PANE3:
        pane3->key_input(c);
        break;
      default:
      {
        printf("state %d \n", fstate);
        assert(false);
        break;
      }
    }
  }

  //
  // } else if (c == 'J') {
  //   pane2->cursor_down();
  // } else if (c == 'K') {
  //   pane2->cursor_up();
  // } else if (c == 'h') {
  //   pane3->cursor_down();
  // } else if (c == 'l') {
  //   pane3->cursor_up();
  // } else if (c == 'N') {
  //   pane1->cursor_down();
  //   pane2->cursor_down();
  //   pane3->cursor_down();
  // } else if (c == 'P') {
  //   pane1->cursor_up();
  //   pane2->cursor_up();
  //   pane3->cursor_up();
  // } else if (c == ' ') {
  //   ToggleList::toggle();
  // } else {
  //   inc = c;
  // }

  std::string str = slankdev::fs("1[%zd] 2[%zd] 3[%zd] focus[",
      pane1->cur(),
      pane2->cur(),
      pane3->cur() );

  std::string ss;
  switch (fstate) {
    case PANE1: ss = "PANE1"; break;
    case PANE2: ss = "PANE2"; break;
    case PANE3: ss = "PANE3"; break;
    default: assert(false);
  }

  str += ss + "]";

  sline->print(str.c_str());
}


