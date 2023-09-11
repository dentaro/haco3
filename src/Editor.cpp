
#include "Editor.h"



Editor::Editor() {
    // コンストラクタの実装
    // ...
}

void Editor::abAppend(struct abuf *ab, const char *s, int len) {
  char *newb = (char *)realloc(ab->b, ab->len + len);
  if (newb == NULL) return;
  memcpy(&newb[ab->len], s, len);
  ab->b = newb;
  ab->len += len;
}

// // カーソル位置(cx)を行の表示位置(rx)に変換する関数
int Editor::editorRowCxToRx(erow *row, int cx) {
  int rx = 0; // カーソル位置(cx)を示す変数(rx)を初期化

  // カーソル位置(cx)までの文字数を計算
  int j = 0;

  while (j <= cx) {
    if ((row->chars[j] & 0xC0) != 0x80) {
      j++;
      rx++;
    }else{//2バイト以上のもじ
      j+=3;
      rx+=2;
    }
  }
  return rx-1; // 行の表示位置(rx)を返す
}


// int getMBCLength(const char* str) {
//   if ((*str & 0x80) == 0) {
//     return 1; // 1バイト文字
//   } else if ((*str & 0xE0) == 0xC0) {
//     return 2; // 2バイト文字
//   } else if ((*str & 0xF0) == 0xE0) {
//     return 3; // 3バイト文字
//   } else if ((*str & 0xF8) == 0xF0) {
//     return 4; // 4バイト文字
//   }
//   return 0; // 不明な形式の文字
// }


void Editor::editorScroll() {

  E.rx = 0; // カーソル位置(rx)を初期化

  // カーソル位置(cx)を行の表示位置(rx)に変換し、E.rxに格納
  if (E.cy < E.numrows) {
    E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
  }

  E. screenrow = getRx() - E.coloff; // カーソルの表示位置をスクリーン座標に変換
  E. screencol = getCy() - E.rowoff; // カーソルの表示位置をスクリーン座標に変換

  if (E. screenrow < 0) {
    E.coloff += E. screenrow; // カーソルが画面外に出た場合、coloffを調整してスクロール
  } else if (E. screenrow >= E.screencols) {
    E.coloff += E. screenrow - E.screencols + 1; // カーソルが画面外に出た場合、coloffを調整してスクロール
  }

  if (E. screencol < 0) {
    E.rowoff += E. screencol; // カーソルが画面外に出た場合、rowoffを調整してスクロール
  } else if (E. screencol >= E.screenrows) {
    E.rowoff += E. screencol - E.screenrows + 1; // カーソルが画面外に出た場合、rowoffを調整してスクロール
  }

}



void Editor::editorDrawRows(struct abuf *ab) {

  int y;
  for (y = 0; y < E.screenrows; y++) {
    int filerow = y + E.rowoff;
    if (filerow >= E.numrows) {
      if (E.numrows == 0 && y == E.screenrows / 3) {
        char welcome[80];
        int welcomelen = snprintf(welcome, sizeof(welcome),
          "Kilo editor -- version %s", KILO_VERSION);
        if (welcomelen > E.screencols) welcomelen = E.screencols;
        int padding = (E.screencols - welcomelen) / 2;
        if (padding) {
          abAppend(ab, "~", 1);
          padding--;
        }
        while (padding--) abAppend(ab, " ", 1);
        abAppend(ab, welcome, welcomelen);
      } else {
        abAppend(ab, "~", 1);
      }
    } else {
      // int len = E.row[filerow].rsize - E.coloff;
      // if (len < 0) len = 0;
      // if (len >= E.screencols) len = E.screencols - 1;
      // // 新たに追加：3 バイト文字が途中で切れないようにするために、
      // // len を 3 の倍数に切り上げる処理を行います。
      // len = (len + 2) / 3 * 3;
      // abAppend(ab, &E.row[filerow].render[E.coloff], len);


      int len = E.row[filerow].rsize - E.coloff;
      if (len < 0) len = 0;
      if (len >= E.screencols) len = E.screencols - 1;
      abAppend(ab, &E.row[filerow].render[E.coloff], len);
    }

    // 新しい行が挿入された場合、それ以降の行も再描画
    if (y < E.screenrows - 1 && filerow < E.numrows - 1) {
      abAppend(ab, "\r\n", 2);
    }
  }
  abAppend(ab, "\n", 1);//ダミー文字を入れる（終端子ごと後で消す）
}


void Editor::abFree(struct abuf *ab) {
  free(ab->b);
}

void Editor::editorMoveCursor(int key) {
  erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  switch (key) {
    case ARROW_LEFT:
      if (E.cx != 0) {
        // カーソルが2バイト目または3バイト目にある場合、1バイト分戻る
        if (E.cx >= 3 && (row->chars[E.cx - 1] & 0xC0) == 0x80) {
          E.cx -= 3;
        } else {
          E.cx--;
        }
      } else if (E.cy > 0) {
        E.cy--;
        E.cx = E.row[E.cy].size;
      }
      break;
  case ARROW_RIGHT:
    if (row && E.cx < row->size) {
        if (E.cx <= row->size - 2 && (row->chars[E.cx + 1] & 0xC0) == 0x80) {
          E.cx += 3; // 3バイト文字の場合は3つ進む
        } else {
          E.cx++;
        }
      } else if (row && E.cx == row->size) {
      E.cy++;
      E.cx = 0;
    }
    break;
    case ARROW_UP:
      if (E.cy != 0) {
        E.cy--;
      }
      break;
    case ARROW_DOWN:
      if (E.cy < E.numrows) {
        E.cy++;
      }
      break;
  }
  
  row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  int rowlen = row ? row->size : 0;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }
}


// 既存の文字列を解放してから新しいメモリを確保する
// void Editor::editorRowInsertChar(erow *row, int at, int c) {
//   if (at < 0 || at > row->size) at = row->size;

//   // 既存のメモリを解放
//   free(row->chars);

//   // 新しいメモリを確保
//   row->chars = (char *)malloc(row->size + 2);

//   // メモリの確保が成功したかを確認
//   if (row->chars == NULL) {
//     // メモリの確保に失敗
//     exit(1); // ここでプログラムを終了させるか、適切なエラーハンドリングを行う
//   }

//   memmove(&row->chars[at + 1], &row->chars[at], row->size - at);
//   row->size++;
//   row->chars[at] = c;
//   row->chars[row->size] = '\0';
//   editorUpdateRow(row);
//   E.dirty++;
// }


void Editor::editorRowInsertChar(erow *row, int at, int c) {
  if (at < 0 || at > row->size) {
    at = row->size; // at が有効な範囲外の場合、末尾に挿入
  }

  if ((c & 0xC0) == 0x80) { // 3バイト文字の場合
    row->chars = (char *)realloc(row->chars, row->size + 3); // 3バイト追加
    memmove(&row->chars[at + 3], &row->chars[at], row->size - at + 1); // 3バイト分シフト
    row->chars[at] = 0xE0 | ((c >> 12) & 0x0F); // 3バイト目
    row->chars[at + 1] = 0x80 | ((c >> 6) & 0x3F); // 2バイト目
    row->chars[at + 2] = 0x80 | (c & 0x3F); // 1バイト目
    row->size += 3;
  } else { // 1バイト文字の場合
    row->chars = (char *)realloc(row->chars, row->size + 2); // 2バイト追加
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1); // 2バイト分シフト
    row->chars[at] = c; // 挿入
    row->size += 1;
  }

  editorUpdateRow(row);
  E.dirty++;
}

// void Editor::editorRowInsertChar(erow *row, int at, int c) {
//   if (at < 0 || at > row->size) at = row->size;
//   row->chars = (char *)realloc(row->chars, row->size + 2);
//   memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
//   row->size++;
//   row->chars[at] = c;
//   editorUpdateRow(row);
//   E.dirty++;
// }

//改行付きで送られてくるもの（空白行など含む生データ）
void Editor::editorInsertRow2(int at, const char *s, size_t len) {
  if (at < 0 || at > E.numrows) return;
  E.row = (erow *)realloc(E.row, sizeof(erow) * (E.numrows + 1));
  memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numrows - at));
  E.row[at].size = len;
  E.row[at].chars = (char *)malloc(len + 1);
  memcpy(E.row[at].chars, s, len);
  E.row[at].chars[len] = '\0';
  E.row[at].rsize = 0;
  E.row[at].render = NULL;
  editorUpdateRow(&E.row[at]);
  E.numrows++;
  E.dirty++;
}


void Editor::editorInsertRow(int at, const char *s, size_t len) {
  if (at < 0 || at > E.numrows) return;  // 挿入位置が範囲外なら何もせずに関数を抜ける

  E.row = (erow *)realloc(E.row, sizeof(erow) * (E.numrows + 1));  // 行データのメモリ領域を拡張

  for (int i = E.numrows; i > at; i--) {
    E.row[i] = E.row[i - 1];  // 指定位置以降の行を1つずつ後ろにずらす
  }

  E.row[at].size = len;  // 新しい行のサイズを設定
  // E.row[at].size = len * 3;  // 新しい行のサイズを設定

  E.row[at].chars = new char[len + 1];  // 新しい行の文字列を格納するメモリを確保

  for (size_t i = 0; i < len; i++) {
    E.row[at].chars[i] = s[i];  // 渡された文字列を新しい行にコピー
  }

  E.row[at].chars[len] = '\0';  // 文字列の終端を設定

  E.row[at].rsize = 0;  // 行の描画サイズをリセット
  E.row[at].render = NULL;  // レンダリングデータをNULLに設定
  editorUpdateRow(&E.row[at]);  // 行データを更新

  E.numrows++;  // 行数を増やす
  E.dirty++;  // 変更フラグを立てる
}

// void Editor::editorInsertChar(int c) {
//   if (E.cy == E.numrows) {
//     editorInsertRow(E.numrows, "", 0);
//   }
//   editorRowInsertChar(&E.row[E.cy], E.cx, c);
//   E.cx++;
// }
void Editor::editorInsertChar(int c) {
  if (E.cy == E.numrows) {
    editorInsertRow(E.numrows, "", 0);
  }
  editorRowInsertChar(&E.row[E.cy], E.cx, c);
  E.cx++;
}

// カーソル位置が行の先頭の場合、新しい行を挿入
// それ以外の場合、現在の行を分割し、新しい行を挿入
// void Editor::editorInsertNewline() {
//   if (E.cx == 0) { // カーソルが行の先頭にある場合
//     editorInsertRow(E.cy, "", 0); // 空行を挿入
//   } else { // カーソルが行の途中にある場合
//     erow *row = &E.row[E.cy];
//     editorInsertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx); // 現在の行を分割し、新しい行を挿入
//     row = &E.row[E.cy];
//     row->size = E.cx; // 分割された行のサイズを更新
//     row->chars[row->size] = '\0'; // 行の終端に終端文字を追加
//     editorUpdateRow(row); // 行の更新を反映
//   }
//   E.cy++; // カーソルを次の行に移動
//   E.cx = 0; // カーソルを行の先頭に移動
// }

void Editor::editorInsertNewline() {
  if (E.cx == 0) { // カーソルが行の先頭にある場合
    editorInsertRow(E.cy, "", 0); // 空行を挿入
  } else { // カーソルが行の途中にある場合
    erow *row = &E.row[E.cy];
    // 新しい行に挿入する文字列のバイト数を計算
    size_t split_size = row->size - E.cx;
    editorInsertRow(E.cy + 1, &row->chars[E.cx], split_size);
    row = &E.row[E.cy];
    // 分割された行のサイズを更新
    row->size = E.cx;
    row->chars[row->size] = '\0';
    editorUpdateRow(row); // 行の更新を反映
  }
  E.cy++; // カーソルを次の行に移動
  E.cx = 0; // カーソルを行の先頭に移動
}

void Editor::editorRowDelChar(erow *row, int at) {
  if (at < 0 || at >= row->size) return;

  if ((row->chars[at] & 0xC0) == 0x80) {
    // 削除位置が3バイト文字の途中なら、3バイト目を削除
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    row->chars[row->size] = '\0';
  } else if (at > 0 && (row->chars[at - 1] & 0xC0) == 0xC0) {
    // 削除位置が2バイト文字の途中なら、2バイト目を削除
    memmove(&row->chars[at - 1], &row->chars[at], row->size - at);
    row->size--;
    row->chars[row->size] = '\0';
  } else {
    // 1バイト文字の場合
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    row->chars[row->size] = '\0';
  }

  editorUpdateRow(row);
  E.dirty++;
}

// void Editor::editorRowDelChar(erow *row, int at) {
//   if (at < 0 || at >= row->size) return;
//   memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
//   row->size--;
//   editorUpdateRow(row);
//   E.dirty++;
// }

void Editor::editorRowAppendString(erow *row, char *s, size_t len) {
  row->chars = (char *)realloc(row->chars, row->size + len + 1);
  memcpy(&row->chars[row->size], s, len);
  row->size += len;
  row->chars[row->size] = '\0';
  editorUpdateRow(row);
  E.dirty++;
}

void Editor::editorFreeRow(erow *row) {
  free(row->render);
  free(row->chars);
}

// void Editor::editorDelRow(int at) {
//   if (at < 0 || at >= E.numrows) return;
//   editorFreeRow(&E.row[at]);
//   memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));
//   E.numrows--;
//   E.dirty++;
// }
void Editor::editorDelRow(int at) {
  if (at < 0 || at >= E.numrows) return;
  editorFreeRow(&E.row[at]);  // 行を削除する前にメモリを解放
  memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));
  E.numrows--;
  E.dirty++;
}



void Editor::editorDelChar() {
  if (E.cy == E.numrows) return;
  if (E.cx == 0 && E.cy == 0) return;
  erow *row = &E.row[E.cy];
  if (E.cx > 0) {
    editorRowDelChar(row, E.cx - 1);
    E.cx--;
  } else {
    E.cx = E.row[E.cy - 1].size;
    editorRowAppendString(&E.row[E.cy - 1], row->chars, row->size);
    editorDelRow(E.cy);
    E.cy--;
  }
}

void Editor::editorDelChar3() {
  // if (E.cy == E.numrows) return;
  // if (E.cx == 0 && E.cy == 0) return;

  // erow *row = &E.row[E.cy];
  // if (E.cx > 0) {


  //     int at = E.cx - 3;
  //     if (at < 0 || at >= row->size) return;
  //     memmove(&row->chars[at], &row->chars[at + 3], row->size - at);
  //     row->size-=3;
  //     editorUpdateRow(row);
  //     E.dirty+=3;

  //     E.cx-=3;


  // } else {
  //   E.cx = E.row[E.cy - 1].size;
  //   editorRowAppendString(&E.row[E.cy - 1], row->chars, row->size);
  //   editorDelRow(E.cy);
  //   E.cy--;
  // }
}

char *Editor::editorRowsToString(int *buflen) {
  int totlen = 0;
  int j;
  for (j = 0; j < E.numrows; j++)
    totlen += E.row[j].size + 1;
  *buflen = totlen;
  char *buf = (char *)malloc(totlen);
  char *p = buf;
  for (j = 0; j < E.numrows; j++) {
    memcpy(p, E.row[j].chars, E.row[j].size);
    p += E.row[j].size;
    *p = '\r';
    p++;
  } *--p = '\0';
  return buf;
}


//----------------------------------------------------

void Editor::editorPageMove(int c, KbdRptParser &Prs) {
  if (c == PAGE_UP&&Prs.getShiftF()) {
          E.cy = E.rowoff;
        } else if (c == PAGE_DOWN&&Prs.getShiftF()) {
          E.cy = E.rowoff + E.screenrows - 1;
          if (E.cy > E.numrows) E.cy = E.numrows;
        }
        int times = E.screenrows;
        while (times--)
          editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
}

void Editor::editorProcessKeypress(int c, fs::FS &fs, fs::FS &SD, KbdRptParser &Prs) {
  if (!c) return;
  switch (c) {
    case ENTER_KEY:
      editorInsertNewline();
      // editorSave(fs);
      break;
    case HOME_KEY:
      E.cx = 0;
      break;
    case END_KEY:
      if (E.cy < E.numrows)
        E.cx = E.row[E.cy].size;
      break;
    if((E.row[E.cy].chars[E.cx - 1] & 0xC0) == 0x80){
      editorDelChar();
      editorDelChar();
      editorDelChar();
    }else{
      editorDelChar();
    }
    case BACKSPACE:

    if((E.row[E.cy].chars[E.cx - 1] & 0xC0) == 0x80){
      editorDelChar();
      editorDelChar();
      editorDelChar();
    }else{
      editorDelChar();
    }
      break;
    case ESCAPE_KEY:
      editorSave(fs);
      delay(200);//ちょっと待つ
      // editorSaveSD(SD);//SDに同時にバックアップ保存する
      // delay(200);//ちょっと待つ
      break;  
    
    case PAGE_UP:
    case PAGE_DOWN:
      {
        editorPageMove(c, Prs);
      //   if (c == PAGE_UP) {
      //     E.cy = E.rowoff;
      //   } else if (c == PAGE_DOWN) {
      //     E.cy = E.rowoff + E.screenrows - 1;
      //     if (E.cy > E.numrows) E.cy = E.numrows;
      //   }
      //   int times = E.screenrows;
      //   while (times--)
      //     editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      editorMoveCursor(c);
      break;
    default:
      editorInsertChar(c);
      editorSetStatusMessage("");
      break;
  }
}

void Editor::editorSetStatusMessage(const char *fmt, ...) {
  // va_list ap;
  // va_start(ap, fmt);
  // vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
  statusMessage = String(E.statusmsg);
  // va_end(ap);
  E.statusmsg_time = time(NULL);
}

void Editor::editorDrawStatusBar(LovyanGFX& tft, struct abuf *ab) {

  tft.setCursor(0, 208);
  tft.setTextColor(HACO3_C8);
  tft.println(String(E.filename)+String(E.numrows)+"lines");
  tft.print("row["+ String(getRx())+ "] col[" +String(getCy())+"]");
  // tft.print("cx["+ String(E.cx)+ "] rx[" +String(E.rx)+"]");
  tft.print(statusMessage);
}
int Editor::getCy(){
  return E.cy;
}
int Editor::getCx(){
  return E.cx;
}
int Editor::getRx(){
  return E.rx;
}

int Editor::getScreenRow(){
  return E.screenrow;
}
int Editor::getScreenCol(){
  return E.screencol;
}

void Editor::editorDrawMessageBar(LovyanGFX& tft, struct abuf *ab) {
  tft.setTextColor(HACO3_C4);
  tft.print(E.statusmsg);
  tft.print(E.statusmsg_time);
  // int msglen = strlen(E.statusmsg);
  // if (msglen > E.screencols) msglen = E.screencols;
  // if (msglen && time(NULL) - E.statusmsg_time < 5)
  //   abAppend(ab, E.statusmsg, msglen);
}

void Editor::editorRefreshScreen(LovyanGFX& tft) {
 
  struct abuf ab = ABUF_INIT;
  // screen.fillScreen(TFT_DARKGREEN);
  tft.fillScreen(HACO3_C1);
  tft.setTextSize(1);//サイズ
  tft.setFont(&lgfxJapanGothic_12);//日本語可,等幅
  // tft.setTextSize(1, 1);
  // tft.setFont(&fonts::Font0);//6*8
  tft.setCursor(0, 1);//位置
  tft.setTextWrap(true);
  // screen.setTextColor(0x00FFFFU, 0x000033U);
  tft.setTextColor( TFT_WHITE);
  // tft.setTextColor( TFT_WHITE , TFT_BLACK );

  tft.fillScreen(HACO3_C1);//コンソール画面をリフレッシュ
  editorScroll();

  editorDrawRows(&ab);

  //終端子をダミー文字ごと消す処理
  if (ab.len > 0) {
    ab.len--; // バッファの長さを1つ減らす
    ab.b[ab.len] = '\0'; // 新しい終端文字を追加
  }
  tft.fillRect(getScreenRow() * 6, getScreenCol() * 13, 6, 13, HACO3_C8); // カーソルを表示

  tft.setCursor(0, 0);
  tft.setTextColor(HACO3_C7);
  tft.println(ab.b);
  editorDrawStatusBar(tft, &ab);
  // editorDrawMessageBar(tft,&ab);
  abFree(&ab);

}

void Editor::editorUpdateRow(erow *row) {
  int tabs = 0; // タブ文字の数をカウントする変数
  int j;
  for (j = 0; j < row->size; j++) {
    if (row->chars[j] == '\t') tabs++; // タブ文字を発見したらカウントを増やす
  }
  
  free(row->render); // 以前の描画データを解放
  row->render = (char *)malloc(row->size + tabs*(KILO_TAB_STOP - 1) + 1); // 描画データのメモリを確保
  
  
  int idx = 0; // 描画データのインデックス
  
  for (j = 0; j < row->size; j++) {
    if (row->chars[j] == '\t') {
      row->render[idx++] = ' '; // タブ文字をスペースに置換
      
      while (idx % KILO_TAB_STOP != 0) row->render[idx++] = ' '; // タブ幅分のスペースを追加
    } else {
      row->render[idx++] = row->chars[j]; // 通常の文字はそのままコピー
    }
  }
  
  row->render[idx] = '\0'; // 描画データの終端を設定
  row->rsize = idx; // 描画データのサイズを更新
}

// void Editor::editorUpdateRow(erow *row) {
//     int tabs = 0;
//     for (int j = 0; j < row->size; j++) {
//         if (row->chars[j] == '\t') tabs++;
//     }

//     int render_size = row->size + tabs * (KILO_TAB_STOP - 1) + 1;

//     char *render = (char *)malloc(render_size);
//     int idx = 0;

//     for (int j = 0; j < row->size; j++) {
//         if (row->chars[j] == '\t') {
//             render[idx++] = ' ';
//             while (idx % KILO_TAB_STOP != 0) render[idx++] = ' ';
//         } else {
//             render[idx++] = row->chars[j];
//         }
//     }

//     render[idx] = '\0';

//     free(row->render);
//     row->render = render;
//     row->rsize = idx;
// }

//日本語のみ消去
// void Editor::editorUpdateRow(erow *row) {
//     free(row->render); // 以前の描画データを解放

//     // 描画データのメモリを確保
//     row->render = (char *)malloc(row->size * 4 + 1);

//     int idx = 0; // 描画データのインデックス

//     for (int j = 0; j < row->size; j++) {
//         unsigned char c = row->chars[j];
//         if (c < 128 || c == 0x07 || c == 0x0B || c == 0x0C || c == 0x0E || c == 0x0F) {
//             row->render[idx++] = (char)c;
//         } else {
//             // 2バイト目以降のバイトは0x80以上の値を持つので、それをチェックしてスキップ
//             while ((j+1 < row->size) && ((row->chars[j+1] & 0xC0) == 0x80)) j++;
//             continue;
//         }
//     }

//     row->render[idx] = '\0'; // 描画データの終端を設定
//     row->rsize = idx; // 描画データのサイズを更新
// }




void Editor::initEditor(LovyanGFX& tft) {
  E.cx = 0;
  E.cy = 0;
  E.rx = 0;
  E.rowoff = 0;
  E.coloff = 0;
  E.screenrows = 16;
  E.screencols = 38;
  E.numrows = 0;
  E.row = NULL;
  E.dirty = 0;
  E.filename = NULL;
  E.statusmsg[0] = '\0';
  E.statusmsg_time = 0;
  E.screenrows -= 2;

  // screen.fillScreen(TFT_DARKGREEN);
  tft.fillScreen(HACO3_C1);
  tft.setTextSize(1);//サイズ
  tft.setFont(&lgfxJapanGothic_12);//日本語可,等幅
  // tft.setTextSize(1, 1);
  // tft.setFont(&fonts::Font0);//6*8
  tft.setCursor(0, 1);//位置
  tft.setTextWrap(true);
  // screen.setTextColor(0x00FFFFU, 0x000033U);
  tft.setTextColor( TFT_WHITE);
  // tft.setTextColor( TFT_WHITE , TFT_BLACK );
}

void Editor::readFile(fs::FS &fs, const char * path) {
   File file = fs.open(path);
   while(file.available()) file.read();
  //  while(file.available()) Serial.print(file.read());
}


void Editor::editorOpen(fs::FS &fs, const char *filename) {
  E.filename = strdup(filename);
  if (fs.exists(filename)) {
    File file = fs.open(filename, FILE_READ);
    if (!file) {
      Serial.println("Failed to open file for reading");
      return;
    }

    String line;
    bool wasLastCharNewline = true; // 最初は改行として扱う

    while (file.available()) {
      char c = file.read();
      if (c == '\n' || c == '\r') {
        if (!wasLastCharNewline) {
          editorInsertRow2(E.numrows, line.c_str(), line.length());
          wasLastCharNewline = true;
        }
        line = "";
      } else {
        line += c;
        wasLastCharNewline = false;
      }
    }

    if (line.length() > 0) {
      editorInsertRow2(E.numrows, line.c_str(), line.length());
    }

    file.close();
  }
}


void Editor::editorSave(fs::FS &fs) {
  if (E.filename == NULL) return;
  File fp = fs.open(E.filename, FILE_WRITE);
  
  if (!fp) {
    editorSetStatusMessage("Failed to open file");
    return;
  }

  for (int i = 0; i < E.numrows; i++) {
    // 行末の改行を削除してから書き出す
    int len = strlen(E.row[i].chars);
    if (len > 0 && E.row[i].chars[len - 1] == '\n') {
      E.row[i].chars[len - 1] = '\0';
    }

    // ファイルに書き出す際にプラットフォームに合わせた改行を使用
    // 例: Windowsの場合はCRLF、他の場合はLF
    #ifdef _WIN32
      fp.print(E.row[i].chars); // WindowsではCRLFを使う
    #else
      fp.println(E.row[i].chars); // 他のプラットフォームではLFを使う
    #endif

    if (fp) {
      editorSetStatusMessage("File written");
    } else {
      editorSetStatusMessage("Failed to write file!");
      fp.close();
      return;
    }
  }

  E.dirty = 0;
  fp.close();
}


void Editor::writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void Editor::editorSaveSD(fs::FS &fs) {//SDに保存する
  if (E.filename == NULL) return;
  // ファイルパスをコピーして変更可能にする
  char filePath[strlen(E.filename) + 1];
  strcpy(filePath, E.filename);

  // ファイルパスをスラッシュで分割する
  char *folderName = strtok(filePath, "/");
  char *fileName = strtok(NULL, "/");

  // フォルダを作成
  String folderPath = "/" + String(folderName);
  if (!fs.exists(folderPath)) {
    fs.mkdir(folderPath);
  }

  // ファイルパスを結合
  String filePathWithFolder = folderPath + "/" + String(fileName);

  // 編集中のデータを文字列に変換
  int len;
  char *buf = editorRowsToString(&len);

  // ファイルを保存
  writeFile(fs, filePathWithFolder.c_str(), buf);

  free(buf);
}

// int Editor::getTargetTime() {
//   return targetTime;
// }

// void Editor::setTargetTime(int _targetTime) {
//     targetTime = _targetTime;
// }

void Editor::update(LovyanGFX& tft, fs::FS &fs, fs::FS &SD, KbdRptParser &Prs) {

    int _keyNo = Prs.getKeycode();
    // int _asciiNo = Prs.getasciicode();

    editorRefreshScreen(tft);

    if(Prs.getKeyDownMillis()!=-1){

      keyMillis = millis() - Prs.getKeyDownMillis();
      if(keyMillis>=0 && keyMillis <= 50){//50ミリ秒だけキーイベントを取る
        editorProcessKeypress(_keyNo, fs, SD, Prs);//キーコードとセーブ先メディアを渡す　初めに一回押される
        // delay(100);
      }else if( keyMillis > 50 && keyMillis <= keywaitTime ){
        //何もしない
      }else if(keyMillis > keywaitTime){
        editorProcessKeypress(_keyNo ,fs, SD, Prs);//キーアップするまでイベントを取る
      }

    }else if(Prs.getKeyDownMillis()==-1){//キーが押されていない
      //何もしない
    }



    // if(targetTime == 0){
    //   editorProcessKeypress(_keyNo, fs, SD);//キーコードとセーブ先メディアを渡す　初めに一回押される
    //   targetTime = millis() + keywaitTime;
    // }
    
    // if(targetTime == -1 || millis() <= targetTime){//keywaitTime中は
    //   //何もしない
    // }

    // if(millis() > targetTime){
    //   editorProcessKeypress(_keyNo ,fs, SD);//キーアップするまで続く
    // }
}