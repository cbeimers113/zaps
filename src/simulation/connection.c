#include "connection.h"

// Draws a line connecting two points, with a circle at the source and an arrow at the target
void connection_render(Connection *connection, uint32_t color, uint32_t *buffer)
{
    draw_line(buffer, connection->sx, connection->sy, connection->tx, connection->ty, color);
    fill_circle(buffer, connection->sx, connection->sy, 5, color);
    draw_line(buffer, connection->tx, connection->ty, connection->alx, connection->aly, color);
    draw_line(buffer, connection->tx, connection->ty, connection->arx, connection->ary, color);
}