#pragma once

namespace tt {
namespace renderer {

typedef struct {
    float x, y, z;
    float u, v;
} vertex;


void startup(void);
void shutdown(void);

void push_vertex(vertex *vertex);
void do_render(void);

} /* namespace renderer */
} /* namespace tt */
