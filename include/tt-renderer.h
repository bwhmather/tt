typedef struct {
    float x, y, z;
    float u, v;
} TTVertex;


void tt_renderer_startup(void);
void tt_renderer_shutdown(void);

void tt_renderer_push_vertex(TTVertex *vertex);
void tt_renderer_do_render(void);

