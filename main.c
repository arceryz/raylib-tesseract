#include <raylib.h>
#include <raymath.h>

int main() {
    InitWindow(800, 800, "Tesseract");
    Camera3D camera = { { 4, 4, 4 }, { 0, 0, 0 }, { 0, 0, 1 }, 50.0, CAMERA_PERSPECTIVE };

    // Find the coordinates by setting XYZW to +-1.
    Vector4 tesseract[16] = {
        {  1,  1,  1, 1 }, {  1,  1,  1, -1 }, 
        {  1,  1, -1, 1 }, {  1,  1, -1, -1 },
        {  1, -1,  1, 1 }, {  1, -1,  1, -1 }, 
        {  1, -1, -1, 1 }, {  1, -1, -1, -1 },
        { -1,  1,  1, 1 }, { -1,  1,  1, -1 }, 
        { -1,  1, -1, 1 }, { -1,  1, -1, -1 },
        { -1, -1,  1, 1 }, { -1, -1,  1, -1 }, 
        { -1, -1, -1, 1 }, { -1, -1, -1, -1 },
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        float rotation = DEG2RAD * 45.0f * GetTime();
        Vector3 transformed[16];
        float w_values[16];

        for (int i = 0; i < 16; i++) {
            Vector4 p = tesseract[i];

            // Rotate the XW part of the vector.
            Vector2 xw_rot = Vector2Rotate((Vector2){ p.x, p.w }, rotation);
            p.x = xw_rot.x;
            p.w = xw_rot.y;

            // Projection from XYZW to XYZ from perspective point (0, 0, 0, 3).
            // Essentially: Trace a ray from (0, 0, 0, 3) > p and continue until W = 0.
            float c = 3 / (3 - p.w);
            p.x = c * p.x;
            p.y = c * p.y;
            p.z = c * p.z;

            // Split XYZ coordinate and W values for drawing later.
            transformed[i] = (Vector3){ p.x, p.y, p.z };
            w_values[i] = p.w;
        }
    
        for (int i = 0; i < 16; i++) {
            // Draw spheres to indicate the W value.
            DrawSphere(transformed[i], fabs(w_values[i] * 0.1), DARKBLUE);

            for (int j = 0; j < 16; j++) {
                // Two points are connected if they differ by 1 coordinate.
                // This way we dont have to keep an edge list.
                Vector4 v1 = tesseract[i];
                Vector4 v2 = tesseract[j];
                int diff = (int)(v1.x == v2.x) + (int)(v1.y == v2.y) + (int)(v1.z == v2.z) + (int)(v1.w == v2.w);

                // Draw only differing by 1 coordinate and the lower index only (duplicate lines).
                if (diff == 3 && i < j) {
                    DrawLine3D(transformed[i], transformed[j], BLUE);
                }
            }
        }
        
        EndMode3D();
        EndDrawing();
    }
}
