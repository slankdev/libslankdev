
#pragma once

#include <stdint.h>
#include <stddef.h>

#include "StatusLine.h"
#include "TextPane.h"
#include "ToggleListPane.h"



class TuiFrontend {
 public:
  TextPane       *pane1;
  ToggleListPane *pane2;
  TextPane       *pane3;
  Statusline     *sline;

 public:
  TuiFrontend();
  void init();
  void refresh();
  virtual ~TuiFrontend();
};



/*
 * Function Implementation
 */
TuiFrontend::TuiFrontend()
  : pane1(nullptr)
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
  pane1 = new TextPane      (0, sublines*0+1, COLS, sublines-1);
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


