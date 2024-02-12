//---------------------------------------
// Program: line_test.cpp
// Purpose: Compare the speeds of three classic
//          line drawing algorithms.
// Author:  John Gauch
// Date:    March 2010
//---------------------------------------
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

// Define global variables
#define SIZE 1000
int Image[SIZE][SIZE];

//------------------------------------------------------------
// Draw line in 2D image using simple DDA algorithm
//------------------------------------------------------------
void draw_simple_DDA(int x1, int y1, int x2, int y2, int value)
{
   // Calculate step size
   int adx = abs(x2 - x1);
   int ady = abs(y2 - y1);
   int length = (adx > ady) ? adx : ady;
   float dx = (x2 - x1) / (float)length;
   float dy = (y2 - y1) / (float)length;

   // Draw pixels on line
   float x = x1 + 0.5;
   float y = y1 + 0.5;
   int i;
   for (i = 0; i <= length; i++)
   {
      Image[(int)y][(int)x] = value;
      x += dx;
      y += dy;
   }
}

//------------------------------------------------------------
// Draw line in 2D image using fast DDA algorithm
//------------------------------------------------------------
void draw_fast_DDA(int x1, int y1, int x2, int y2, int value)
{
   // Calculate step size
   int dx = x2 - x1;
   int dy = y2 - y1;
   int adx = abs(dx);
   int ady = abs(dy);
   int x, y;

   // Handle slope (0..1)
   if (ady < adx)
   {
      float step = (float)dy / (float)dx;
      float y = y1 + 0.5;
  
      if (dx > 0)
         for (x=x1; x<=x2; x++, y += step)
            Image[(int)y][x] = value;
      else
         for (x=x1; x>=x2; x--, y -= step)
            Image[(int)y][x] = value;
   }

   // Handle slope [1..inf)
   else 
   {
      float step = (float)dx / (float)dy;
      float x = x1 + 0.5;

      if (dy > 0)
         for (y=y1; y<=y2; y++, x += step)
            Image[y][(int)x] = value;
      else
         for (y=y1; y>=y2; y--, x -= step)
            Image[y][(int)x] = value;
   }
}

//------------------------------------------------------------
// Draw line in 2D image using Bresenham's algorithm
//------------------------------------------------------------
void draw_bresenham(int x1, int y1, int x2, int y2, int value)
{
   // Calculate step size
   int dx = x2 - x1;
   int dy = y2 - y1;
   int adx = abs(dx);
   int ady = abs(dy);
   int x, y;

   // Handle slope (0..1)
   if (ady < adx)
   {
      // Draw line with dx > 0
      if (dx > 0)
      {
         int error = dx/2;
         int step = (dy > 0) ? 1 : -1;
         for (x=x1, y=y1; x<=x2; x++)
         {
            Image[y][x] = value;
            error -= ady;
            if (error < 0)
            {
               error += dx;
               y += step;
            }
         }
      }
      
      // Draw line with dx < 0
      else 
      {
         int error = -dx/2;
         int step = (dy > 0) ? 1 : -1;
         for (x=x1, y=y1; x>=x2; x--)
         {
            Image[y][x] = value;
            error -= ady;
            if (error < 0)
            {
               error -= dx;
               y += step;
            }
         }
      }
   }

   // Handle slope [1..inf)
   else 
   {
      // Draw line with dy > 0
      if (dy > 0)
      {
         int error = dy/2;
         int step = (dx > 0) ? 1 : -1;
         for (x=x1, y=y1; y<=y2; y++)
         {
            Image[y][x] = value;
            error -= adx;
            if (error < 0)
            {
               error += dy;
               x += step;
            }
         }
      }

      // Draw line with dy < 0
      if (dy < 0)
      {
         int error = -dy/2;
         int step = (dx > 0) ? 1 : -1;
         for (x=x1, y=y1; y>=y2; y--)
         {
            Image[y][x] = value;
            error -= adx;
            if (error < 0)
            {
               error -= dy;
               x += step;
            }
         }
      }
   }
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Declare local variables
   int x1[SIZE];
   int y1[SIZE];
   int x2[SIZE];
   int y2[SIZE];

   // Generate random lines
   for (int i = 0; i < SIZE; i++)
   {
      x1[i] = rand() % SIZE;
      y1[i] = rand() % SIZE;
      x2[i] = rand() % SIZE;
      y2[i] = rand() % SIZE;
   }

   // Test line drawing function
   Timer t;
   t.start_timer();
   for (int j = 0; j < SIZE; j++)
      for (int i = 0; i < SIZE; i++)
         draw_simple_DDA(x1[i], y1[i], x2[i], y2[i], i);
   t.end_timer();
   printf("Simple DDA: %d lines/sec\n", int(SIZE*SIZE/t.elapsed_cpu()));
   
   // Test line drawing function
   t.start_timer();
   for (int j = 0; j < SIZE; j++)
      for (int i = 0; i < SIZE; i++)
         draw_fast_DDA(x1[i], y1[i], x2[i], y2[i], i);
   t.end_timer();
   printf("Fast DDA: %d lines/sec\n", int(SIZE*SIZE/t.elapsed_cpu()));
   
   // Test line drawing function
   t.start_timer();
   for (int j = 0; j < SIZE; j++)
      for (int i = 0; i < SIZE; i++)
         draw_bresenham(x1[i], y1[i], x2[i], y2[i], i);
   t.end_timer();
   printf("Bresenham: %d lines/sec\n", int(SIZE*SIZE/t.elapsed_cpu()));
   return 0;
}
