
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include "StatusLine.h"
#include "TextPane.h"
#include "ToggleListPane.h"

#include "PacketListPane.h"

enum FocusState {
  PANE1,
  PANE2,
  PANE3,
};

class TuiFrontend {
  FocusState fstate;
 public:
  PacketListPane  pane1;
  ToggleListPane  pane2;
  TextPane        pane3;
  Statusline      sline;

 public:
  TuiFrontend();
  virtual ~TuiFrontend();

  static void init();
  void refresh();

  void key_input(char c);
  void focuse_sw();
};


/*
 * Function Implementation
 */

void TuiFrontend::focuse_sw()
{
  switch (fstate) {
    case PANE1: fstate = PANE2; break;
    case PANE2: fstate = PANE3; break;
    case PANE3: fstate = PANE1; break;
    default: assert(false);
  }
}

#define sublines (LINES/3-1)
TuiFrontend::TuiFrontend()
  : fstate(PANE1)
  , pane1(0, sublines*0+1, COLS, sublines-1)
  , pane2(0, sublines*1  , COLS, sublines  )
  , pane3(0, sublines*2  , COLS, sublines  )
  , sline(0, sublines*3+1, COLS)
{
  pane1.init(stdscr);
  pane2.init(stdscr);
  pane3.init(stdscr);
  sline.init(stdscr);
}
TuiFrontend::~TuiFrontend() {}
void TuiFrontend::init()
{
  initscr();
  noecho();
  scrollok(stdscr, false);
}
void TuiFrontend::refresh()
{
  if (!pane1.packets.empty()) {
    pane1.refresh();
    size_t cur = pane1.cur();
    pane2.set_content(&pane1.packets.at(cur)->details);
    pane3.set_content(&pane1.packets.at(cur)->binarys);
  }

  pane2.refresh();
  pane3.refresh();
  sline.refresh();
}
void TuiFrontend::key_input(char c)
{
  char inc = ' ';

  if (c == '\t') {
    focuse_sw();
  } else {
    switch (fstate) {
      case PANE1:
        pane1.key_input(c);
        break;
      case PANE2:
        pane2.key_input(c);
        break;
      case PANE3:
        pane3.key_input(c);
        break;
      default:
      {
        printf("state %d \n", fstate);
        assert(false);
        break;
      }
    }
  }
  std::string ss;
  switch (fstate) {
    case PANE1: ss = "PANE1"; break;
    case PANE2: ss = "PANE2"; break;
    case PANE3: ss = "PANE3"; break;
    default: assert(false);
  }
  std::string str = slankdev::fs("1[%zd] 2[%zd] 3[%zd] focus[%s]",
      pane1.cur(),
      pane2.cur(),
      pane3.cur(),
      ss.c_str()
      );
  sline.print(str.c_str());
}


