export module action;
import <string>;
// c[any motion] dd d[any motion] f n p q r s u w x yy
// y[any motion] A F I J N O P R S X ; / ? % @
// ^b ^d ^f ^g ^u :line-number

export enum Command {
  INVALID = 0,
  ENTER, // ENTER and KEY_ENTER
  
// NORMAL MODE
  NORMAL_MODE, // ESC
  CARET, // ^

  CURSOR_LEFT, // h
  CURSOR_DOWN, // j
  CURSOR_UP, // k
  CURSOR_RIGHT, // l
  
  CURSOR_BEG_NEXT_W, // w
  CURSOR_BEG_PREV_W, // b
  CURSOR_END_CUR_W, // e, optional

  CURSOR_BEG_LINE, // 0
  CURSOR_END_LINE, // $
  REPL_ONE, // r
  DEL_ONE, // x
  UNDO, // u
  REDO, // CTRL+r

  REPL_LINE, // cc

  REP_LAST_CMD, // .

  FINDF, // f
  FINDB, // F
  FINDR, // ;
  FINDRO, // ,

  YANKY, // yy
  YANKW, // yw
  YANKIW, // yiw
  YANKAW, // yaw
  YANKD, // y$
  YANKC, // y^
  YANKTX, // yt
  YANKFX, // yf

  CHANGELINE, // cc
  CHANGEWORD, // cw
  CHANGE_ENDWORD, // ce
  CHANGE_ENDLINE, // c$

  DELETELINE, // dd
  DELETEWORD, // cw
  DELETE_ENDWORD, // ce
  DELETE_ENDLINE, // c$

  PUT, // p

// INSERT
  INSERT_MODE, // i  
  APPEND, // a
  NEWLINE, // o
  NEWLINEABV, // O

// COMMAND
  COMMAND_MODE, // :
  WRITE, // :w
  WRITE_QUIT, // :wq
  QUIT, // :q
  QUITX, // :q!
  READ, // :r
  CURSOR_BEG_FILE, // :0
  CURSOR_END_FILE, // :$

  // MISC
  CYCLEFPREV, // -
  CYCLEFNEXT, // =
  OPENFILE, // CTRL+t
  OPENFILEX, // open file with data x
  CLOSEFILE, // CTRL+w
  WRESIZE, // window resize
  WRESIZEL, // [
  WRESIZER, // ]
};

export struct Action {
  bool isCommand;
  std::string data;
  int multiplier;
  union {
    int keystroke;
    Command command;
  };
};
