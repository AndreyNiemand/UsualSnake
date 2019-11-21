
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define head_pic   '0'
#define tail_pic   'o'
#define fruit_pic  '@'
#define border_pic '#'

#define border_x   60
#define border_y   20

#define score_x    (border_x + 5)
#define score_y    0

enum { playing, pause, win } status = playing;

int fruit_x = 0;
int fruit_y = 0;

enum { left, right, up, down } head_dir = right;
int head_x = 20;
int head_y = 20;

int tail_x [border_x * border_y];
int tail_y [border_x * border_y];
int tail_count = 5;

int score = 0;

void moveTo(int x, int y)
{
  COORD c;
  c.X = x;
  c.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void draw(int x, int y, char graph)
{
  moveTo(x, y);
  putchar(graph);
}

void drawScore()
{
  moveTo(score_x + 7, score_y);
  puts("     ");
  moveTo(score_x + 7, score_y);
  char buf[5];
  _itoa_s(score, buf, 5, 10);
  puts(buf);
}

void setRandomFruit()
{
  fruit_x = rand() % border_x;
  fruit_y = rand() % border_y;

  if (head_x == fruit_x
   && head_y == fruit_y)
  {
    setRandomFruit();
  }

  for (int i = 0; i < tail_count; i = i + 1)
  {
    if (tail_x[i] == fruit_x 
     && tail_y[i] == fruit_y)
    {
      setRandomFruit();
    }
  }
}

int main()
{
  CONSOLE_CURSOR_INFO structCursorInfo;
  GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &structCursorInfo);
  structCursorInfo.bVisible = FALSE;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &structCursorInfo);
  
  moveTo(score_x, score_y);
  puts("Score: ");

  setRandomFruit();

  for (int i = 0; i < tail_count; i = i + 1)
  {
    tail_x[i] = head_x;
    tail_y[i] = head_y;
  }

  for (int x = 0; x < border_x + 1; x = x + 1)
  {
    draw(x, border_y + 1, border_pic);
  }

  for (int y = 0; y < border_y + 1; y = y + 1)
  {
    draw(border_x + 1, y, border_pic);
  }

  for (;;)
  {
    if (tail_count == border_x * border_y)
    {
      status = win;
      break;
    }

    for (int i = 0; i < 1000 / 24; i = i + 1)
    {
      if (head_dir != down && GetAsyncKeyState(VK_UP))
      {
        head_dir = up;
      }
      if (head_dir != up && GetAsyncKeyState(VK_DOWN))
      {
        head_dir = down;
      }
      if (head_dir != right && GetAsyncKeyState(VK_LEFT))
      {
        head_dir = left;
      }
      if (head_dir != left && GetAsyncKeyState(VK_RIGHT))
      {
        head_dir = right;
      }

      if (GetAsyncKeyState(VK_ESCAPE))
      {
        goto exit;
      }

      if (GetAsyncKeyState(VK_SPACE))
      {
        if (status == pause)
        {
          status = playing;
        }
        else if (status == playing)
        {
          status = pause;
        }
      }

      Sleep(1);
    }

    draw(head_x, head_y, ' ');

    draw(fruit_x, fruit_y, ' ');

    for (int i = 0; i < tail_count; i = i + 1)
    {
      draw(tail_x[i], tail_y[i], ' ');
    }

    if (status == playing)
    {
      if (head_x == fruit_x
       && head_y == fruit_y)
      {
        if (tail_count > 0)
        {
          tail_x[tail_count] = tail_x[tail_count - 1];
          tail_y[tail_count] = tail_y[tail_count - 1];
        }
        tail_count = tail_count + 1;

        setRandomFruit();
        score = score + 1;
      }

      if (tail_count > 0)
      {
        for (int i = tail_count - 1; i > 0; i = i - 1)
        {
          tail_x[i] = tail_x[i - 1];
          tail_y[i] = tail_y[i - 1];
        }

        tail_x[0] = head_x;
        tail_y[0] = head_y;
      }

      if (head_dir == left)
      {
        head_x = head_x - 1;
      }
      if (head_dir == right)
      {
        head_x = head_x + 1;
      }

      if (head_dir == up)
      {
        head_y = head_y - 1;
      }
      if (head_dir == down)
      {
        head_y = head_y + 1;
      }

      if (head_x < 0)
      {
        head_x = border_x;
      }
      if (head_x > border_x)
      {
        head_x = 0;
      }

      if (head_y < 0)
      {
        head_y = border_y;
      }
      if (head_y > border_y)
      {
        head_y = 0;
      }

      for (int i = 0; i < tail_count; i = i + 1)
      {
        if (tail_x[i] == head_x
          && tail_y[i] == head_y)
        {
          score = score - (tail_count - i) * 2;
          tail_count = i;
          break;
        }
      }
    }
    draw(head_x, head_y, head_pic);

    draw(fruit_x, fruit_y, fruit_pic);

    drawScore(10, 10);

    for (int i = 0; i < tail_count; ++i)
    {
      draw(tail_x[i], tail_y[i], tail_pic);
    }
  }

exit:
  system("cls");
  if (status == win)
  {
    printf("Great! You won! Your score: %d.", score);
  }
  else 
  {
    printf("Your score: %d.", score);
  }

  return 0;
}


