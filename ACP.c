#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define WIDTH 60
#define HEIGHT 30
#define MAX_OBJS 50

typedef enum { LINE, RECT, CIRCLE, TRIANGLE } ShapeType;

typedef struct {
    ShapeType type;
    int x1, y1, x2, y2, r; // Coordinates or radius
    char sprite;           // '*' or '_'
    int active;            // 1 if exists, 0 if deleted
} Shape;

char canvas[HEIGHT][WIDTH];
Shape scene[MAX_OBJS];
int shape_count = 0;

void clear_canvas() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            canvas[i][j] = ' ';
}

void plot(int x, int y, char c) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        canvas[y][x] = c;
}

// Bresenham's Line Algorithm
void draw_line(int x0, int y0, int x1, int y1, char c) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        plot(x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_rect(int x, int y, int w, int h, char c) {
    draw_line(x, y, x + w, y, c);         // Top
    draw_line(x, y + h, x + w, y + h, c); // Bottom
    draw_line(x, y, x, y + h, c);         // Left
    draw_line(x + w, y, x + w, y + h, c); // Right
}

void draw_circle(int xm, int ym, int r, char c) {
    int x = -r, y = 0, err = 2 - 2 * r;
    do {
        // Adjusting x for terminal aspect ratio (approx 2:1)
        plot(xm - (x * 2), ym + y, c);
        plot(xm - (y * 2), ym - x, c);
        plot(xm + (x * 2), ym - y, c);
        plot(xm + (y * 2), ym + x, c);
        r = err;
        if (r <= y) err += ++y * 2 + 1;
        if (r > x || err > y) err += ++x * 2 + 1;
    } while (x < 0);
}

void render_scene() {
    clear_canvas();
    for (int i = 0; i < MAX_OBJS; i++) {
        if (!scene[i].active) continue;
        Shape s = scene[i];
        if (s.type == LINE) draw_line(s.x1, s.y1, s.x2, s.y2, s.sprite);
        else if (s.type == RECT) draw_rect(s.x1, s.y1, s.x2, s.y2, s.sprite);
        else if (s.type == CIRCLE) draw_circle(s.x1, s.y1, s.r, s.sprite);
        else if (s.type == TRIANGLE) {
            draw_line(s.x1, s.y1, s.x2, s.y2, s.sprite);
            draw_line(s.x2, s.y2, s.x1 - (s.x2 - s.x1), s.y2, s.sprite);
            draw_line(s.x1 - (s.x2 - s.x1), s.y2, s.x1, s.y1, s.sprite);
        }
    }
}

void display() {
    system("clear");
    for (int i = 0; i < WIDTH + 2; i++) printf("-");
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        printf("|");
        for (int j = 0; j < WIDTH; j++) printf("%c", canvas[i][j]);
        printf("|\n");
    }
    for (int i = 0; i < WIDTH + 2; i++) printf("-");
    printf("\n");
}

int main() {
    int choice, id;
    while (1) {
        render_scene();
        display();
        printf("\n1. Add Rect  2. Add Circle  3. Add Line  4. Delete  5. Modify  6. Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 6) break;
        if (choice >= 1 && choice <= 3) {
            id = shape_count++;
            scene[id].active = 1;
            scene[id].sprite = (id % 2 == 0) ? '*' : '_';
            if (choice == 1) {
                scene[id].type = RECT;
                printf("Enter x, y, width, height: ");
                scanf("%d %d %d %d", &scene[id].x1, &scene[id].y1, &scene[id].x2, &scene[id].y2);
            } else if (choice == 2) {
                scene[id].type = CIRCLE;
                printf("Enter center x, y and radius: ");
                scanf("%d %d %d", &scene[id].x1, &scene[id].y1, &scene[id].r);
            } else if (choice == 3) {
                scene[id].type = LINE;
                printf("Enter x1, y1, x2, y2: ");
                scanf("%d %d %d %d", &scene[id].x1, &scene[id].y1, &scene[id].x2, &scene[id].y2);
            }
        } else if (choice == 4) {
            printf("Enter ID to delete (0-%d): ", shape_count-1);
            scanf("%d", &id);
            if (id < MAX_OBJS) scene[id].active = 0;
        } else if (choice == 5) {
            printf("Enter ID to move (0-%d): ", shape_count-1);
            scanf("%d", &id);
            printf("Enter new X and Y: ");
            scanf("%d %d", &scene[id].x1, &scene[id].y1);
        }
    }
    return 0;
}
