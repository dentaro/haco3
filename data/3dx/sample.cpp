
#include <vector>
#include <array>

void camera(float x, float y) {
  if (x != NULL) {
    cam_x = x;
  }
  if (y != NULL) {
    cam_y = y;
  }
}

void add(std::vector<int>& t, int v) {
  t.push_back(v);
}

void del(std::vector<int>& t, int v) {
  for (int i = t.size() - 1; i >= 0; i--) {
    if (t[i] == v) {
      t.erase(t.begin() + i);
    }
  }
}


struct Object {
std::vector<std::array<float, 3>> vertices, t_vertices;
std::vector<std::array<int, 3>> faces;
float x = 0.0f, y = 0.0f, z = 0.0f;
float rx = 0.0f, ry = 0.0f, rz = 0.0f;
float tx = 0.0f, ty = 0.0f, tz = 0.0f;
float ax = 0.0f, ay = 0.0f, az = 0.0f;
float sx = 0.0f, sy = 0.0f, radius = 10.0f, sradius = 10.0f;
float min_x = 100.0f, min_y = 100.0f, min_z = 100.0f;
float max_x = -100.0f, max_y = -100.0f, max_z = -100.0f;
float vx = 0.0f, vy = 0.0f, vz = 0.0f;

int age = 0, health = 2;

bool visible = true, render = true, background = false, collision_x = true,
     collision_y = false, collision_down = false, collision_up = false,
     collision_left = false, ring = false;
};

std::vector<Object> object_list;

Object new_object() {
    Object object;
    object_list.push_back(object);
    return object;
}

// init_player()
Object player = new_object();
player.min_x = -4.5f;
player.min_y = -4.5f;
player.min_z = -4.5f;
player.max_x = 4.5f;
player.max_y = 4.5f;
player.max_z = 4.5f;

player.x = 0.0f;
player.y = 8.0f;
player.z = 15.0f;

player.vx = 0.0f;
player.vy = 0.0f;
player.vz = 0.0f;

// init_light()
float light1_x = 0.1f;
float light1_y = 0.35f;
float light1_z = 0.2f;
float t_light_x = 0.0f;
float t_light_y = 0.0f;
float t_light_z = 0.0f;


void foreach(std::vector<Object*>& t, std::function<void(Object*)> f) {
    for (auto v : t) {
        f(v);
    }
}



bool intersect_bounding_box(const Object& object_a, const Object& object_b) {
    return (object_a.min_x + object_a.x < object_b.max_x + object_b.x) &&
           (object_a.max_x + object_a.x > object_b.min_x + object_b.x) &&
           (object_a.min_y + object_a.y < object_b.max_y + object_b.y) &&
           (object_a.max_y + object_a.y > object_b.min_y + object_b.y) &&
           (object_a.min_z + object_a.z < object_b.max_z + object_b.z) &&
           (object_a.max_z + object_a.z > object_b.min_z + object_b.z);
}

void update_player() {
    float old_x = player.x;
    float old_y = player.y;
    float old_z = player.z;

    player.y += player.vy;
    player.x += player.vx;

    for (auto& object : obstacle_list) {
        if (intersect_bounding_box(player, object)) {
            player.vx = 0;
            player.x = old_x;
        }
    }

    player.z += player.vz;

    for (auto& object : obstacle_list) {
        if (intersect_bounding_box(player, object)) {
            player.vz = 0;
            player.z = old_z;
        }
    }

    player.vx *= k_friction;
    player.vy *= k_friction;
    player.vz *= k_friction;
}

void generate_cam_matrix_transform(float xa, float ya, float za) {
    float sx = sin(xa);
    float sy = sin(ya);
    float sz = sin(za);
    float cx = cos(xa);
    float cy = cos(ya);
    float cz = cos(za);

    cam_mat00 = cz * cy;
    cam_mat10 = -sz;
    cam_mat20 = cz * sy;
    cam_mat01 = cx * sz * cy + sx * sy;
    cam_mat11 = cx * cz;
    cam_mat21 = cx * sz * sy - sx * cy;
    cam_mat02 = sx * sz * cy - cx * sy;
    cam_mat12 = sx * cz;
    cam_mat22 = sx * sz * sy + cx * cy;
}


void update_camera() {
    cam_x = player.x;
    cam_y = player.y;
    cam_z = player.z;

    cam_ax = player.ax;
    cam_ay = player.ay;
    cam_az = player.az;

    generate_cam_matrix_transform(cam_ax, cam_ay, cam_az);
}


Object load_object(std::vector<std::vector<float>> object_vertices,
                   std::vector<std::vector<int>> object_faces,
                   float x = 0.0f, float y = 0.0f, float z = 0.0f,
                   float ax = 0.0f, float ay = 0.0f, float az = 0.0f,
                   bool obstacle = false,
                   ColorMode color_mode = ColorMode::k_colorize_static,
                   int color = 8) {
    Object object;
    object.vertices = object_vertices;
    if (color_mode == ColorMode::k_preset_color) {
        object.faces = object_faces;
    } else {
        object.base_faces = object_faces;
        for (int i = 0; i < object_faces.size(); i++) {
            std::vector<int> face;
            for (int j = 0; j < object_faces[i].size(); j++) {
                face.push_back(object_faces[i][j]);
            }
            object.faces.push_back(face);
        }
    }

    object.radius = 0;
    for (int i = 0; i < object_vertices.size(); i++) {
        std::vector<float> vertex;
        for (int j = 0; j < 3; j++) {
            vertex.push_back(object_vertices[i][j]);
        }
        object.t_vertices.push_back(vertex);
    }

    object.ax = ax;
    object.ay = ay;
    object.az = az;

    transform_object(object);

    set_radius(object);
    set_bounding_box(object);

    object.x = x;
    object.y = y;
    object.z = z;

    object.color = color;
    object.color_mode = color_mode;

    object.obstacle = obstacle;

    if (obstacle) {
        obstacle_list.push_back(object);
    }

    if (color_mode == ColorMode::k_colorize_static ||
        color_mode == ColorMode::k_colorize_dynamic ||
        color_mode == ColorMode::k_multi_color_static) {
        color_faces(object, color);
    }

    return object;
}

#include <cmath>
#include <tuple>

double cam_mat00 = 1, cam_mat01 = 0, cam_mat02 = 0, cam_mat10 = 0, cam_mat11 = 1, cam_mat12 = 0, cam_mat20 = 0, cam_mat21 = 0, cam_mat22 = 1;
double k_screen_scale = 256;
double k_x_center = 64;
double z_max = 200;
double z_clip = 0;
    
std::tuple<double, double, double> rotate_cam_point(double x, double y, double z) {
  return std::make_tuple(x * cam_mat00 + y * cam_mat10 + z * cam_mat20,
                         x * cam_mat01 + y * cam_mat11 + z * cam_mat21,
                         x * cam_mat02 + y * cam_mat12 + z * cam_mat22);
}

std::tuple<double, double> project_point(double x, double y, double z) {
  return std::make_tuple(x * k_screen_scale / z + k_x_center, y * k_screen_scale / z + k_x_center);
}

double project_radius(double r, double z) {
  return r * k_screen_scale / std::abs(z);
}

bool is_visible(Object object) {
  if (object.tz + object.radius > z_max && object.tz - object.radius < z_clip &&
      object.sx + object.sradius > 0 && object.sx - object.sradius < 128 &&
      object.sy + object.sradius > 0 && object.sy - object.sradius < 128)
    return true;
  else
    return false;
}



void new_cube() {
if (frame % period == 1) {
float x = 4 - rand() % 3 * 4;
int col = rand() % 8 + 8;
Object* c = load_object(cube_v, cube_f, x, 4, -20, 0, 0, 0, false, k_colorize_dynamic, col);
c->vx = 0.0f;
c->vy = 0.0f;
c->vz = 0.5f;
}
}



void update_visible(Object* object) {
    object->visible = false;
    float px = object->x - cam_x, py = object->y - cam_y, pz = object->z - cam_z;
    object->tx = 0.0f, object->ty = 0.0f, object->tz = 0.0f;
    rotate_cam_point(px, py, pz, object->tx, object->ty, object->tz);
    object->sx = 0.0f, object->sy = 0.0f;
    project_point(object->tx, object->ty, object->tz, object->sx, object->sy);
    object->sradius = project_radius(object->radius, object->tz);
    object->visible = is_visible(object);
}

void transform_object(Object* object) {
    if (object->visible) {
        generate_matrix_transform(object->ax, object->ay, object->az);
        for (int i = 0; i < object->vertices.size(); ++i) {
            auto& t_vertex = object->t_vertices[i];
            const auto& vertex = object->vertices[i];
            rotate_point(vertex[0], vertex[1], vertex[2], t_vertex[0], t_vertex[1], t_vertex[2]);
        }
    }
}

void cam_transform_object(Object* object) {
    if (object->visible) {
        for (int i = 0; i < object->vertices.size(); ++i) {
            auto& vertex = object->t_vertices[i];
            vertex[0] += (object->x - cam_x);
            vertex[1] += (object->y - cam_y);
            vertex[2] += (object->z - cam_z);
            rotate_cam_point(vertex[0], vertex[1], vertex[2], vertex[0], vertex[1], vertex[2]);
        }
    }
}

void update_light() {
    rotate_cam_point(light1_x, light1_y, light1_z, t_light_x, t_light_y, t_light_z);
}

void update_3d() {
  for (auto& object : object_list) {
    update_visible(object);
    transform_object(object);
    cam_transform_object(object);
    update_light();
  }
}

#include <vector>
#include <algorithm>

using namespace std;

vector<int> quicksort(vector<int>& t, int start = 0, int endi = -1) {
    if (endi == -1) endi = t.size() - 1;
    if (endi - start < 1) return t;
    int pivot = start;
    for (int i = start + 1; i <= endi; i++) {
        if (t[i] <= t[pivot]) {
            if (i == pivot + 1) {
                swap(t[pivot], t[pivot + 1]);
            } else {
                swap(t[pivot], t[pivot + 1]);
                swap(t[pivot + 1], t[i]);
            }
            pivot++;
        }
    }
    t = quicksort(t, start, pivot - 1);
    return quicksort(t, pivot + 1, endi);
}

double timer_value;

void start_timer() {
    timer_value = clock();
}

double stop_timer() {
    return (clock() - timer_value) / (double) CLOCKS_PER_SEC * 1000.0;
}

void shade_trifill(int x1, int y1, int x2, int y2, int x3, int y3, int color1, int color2) {
    x1 &= 0xffff;
    x2 &= 0xffff;
    y1 &= 0xffff;
    y2 &= 0xffff;
    x3 &= 0xffff;
    y3 &= 0xffff;
    
    int nsx, nex;

    if (y1 > y2) {
        std::swap(y1, y2);
        std::swap(x1, x2);
    }
    
    if (y1 > y3) {
        std::swap(y1, y3);
        std::swap(x1, x3);
    }
    
    if (y2 > y3) {
        std::swap(y2, y3);
        std::swap(x2, x3);          
    }

    if (y1 != y2) {
        float delta_sx = static_cast<float>(x3 - x1) / (y3 - y1);
        float delta_ex = static_cast<float>(x2 - x1) / (y2 - y1);
     
        if (y1 > 0) {
            nsx = x1;
            nex = x1;
            int min_y = y1;
        } else { //top edge clip
            nsx = x1 - delta_sx * y1;
            nex = x1 - delta_ex * y1;
            int min_y = 0;
        }
     
        int max_y = std::min(y2, 128);
     
        for (int y = min_y; y < max_y; ++y) {
            if (y % 2 == 0) {
                rectfill(nsx, y, nex, y, color1);
            } else {
                rectfill(nsx, y, nex, y, color2);
            }
            nsx += delta_sx;
            nex += delta_ex;
        }
    } else { //where top edge is horizontal
        nsx = x1;
        nex = x2;
    }
    
    if (y3 != y2) {
        float delta_sx = static_cast<float>(x3 - x1) / (y3 - y1);
        float delta_ex = static_cast<float>(x3 - x2) / (y3 - y2);
     
        int min_y = y2;
        int max_y = std::min(y3, 128);
        
        if (y2 < 0) {
            nex = x2 - delta_ex * y2;
            nsx = x1 - delta_sx * y1;
            min_y = 0;
        }
     
        for (int y = min_y; y <= max_y; ++y) {
            if (y % 2 == 0) {
                rectfill(nsx, y, nex, y, color1);
            } else {
                rectfill(nsx, y, nex, y, color2);
            }
            nex += delta_ex;
            nsx += delta_sx;
        }
    } else { //where bottom edge is horizontal
        rectfill(nsx, y3, nex, y3, color1);
    }
}

void draw_triangle_list() {
    for (int i = 0; i < triangle_list.size(); i++) {
        auto t = triangle_list[i];
        shade_trifill(t.p1x, t.p1y, t.p2x, t.p2y, t.p3x, t.p3y, t.c1, t.c2);
    }
}

std::tuple<float, float, float, float, float, float, float, float, float> 
    three_point_sort(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z) {
    if (p1z > p2z) {
        std::swap(p1z, p2z);
        std::swap(p1x, p2x);
        std::swap(p1y, p2y);
    }
    if (p1z > p3z) {
        std::swap(p1z, p3z);
        std::swap(p1x, p3x);
        std::swap(p1y, p3y);
    }
    if (p2z > p3z) {
        std::swap(p2z, p3z);
        std::swap(p2x, p3x);
        std::swap(p2y, p3y);
    }
    return std::make_tuple(p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z);
}

float lerp(float a, float b, float alpha) {
    return a * (1.0 - alpha) + b * alpha;
}

std::optional<std::tuple<float, float, float>> 
    z_clip_line(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float clip) {
    if (p1z > p2z) {
        std::swap(p1z, p2z);
        std::swap(p1x, p2x);
        std::swap(p1y, p2y);
    }
    if (clip > p1z && clip <= p2z) {
        float alpha = abs((p1z - clip) / (p2z - p1z));
        float nx = lerp(p1x, p2x, alpha);
        float ny = lerp(p1y, p2y, alpha);
        float nz = lerp(p1z, p2z, alpha);
        return std::make_tuple(nx, ny, nz);
    }
    else {
        return std::nullopt;
    }
}

void new_triangle(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, float z, int c1, int c2) {
    triangle_list.push_back({p1x, p1y, p2x, p2y, p3x, p3y, z, c1, c2});
}

void render_object(Object& object) {
    for (int i = 0; i < object.t_vertices.size(); ++i) {
        auto& vertex = object.t_vertices[i];
        vertex[3] = vertex[1] * k_screen_scale / vertex[3] + k_x_center;
        vertex[4] = vertex[2] * k_screen_scale / vertex[3] + k_x_center;
    }

    for (int i = 0; i < object.faces.size(); ++i) {
        auto& face = object.faces[i];

        auto& p1 = object.t_vertices[face[0]];
        auto& p2 = object.t_vertices[face[1]];
        auto& p3 = object.t_vertices[face[2]];

        float p1x = p1[0], p1y = p1[1], p1z = p1[2];
        float p2x = p2[0], p2y = p2[1], p2z = p2[2];
        float p3x = p3[0], p3y = p3[1], p3z = p3[2];

        float cz = 0.01f * (p1z + p2z + p3z) / 3;
        float cx = 0.01f * (p1x + p2x + p3x) / 3;
        float cy = 0.01f * (p1y + p2y + p3y) / 3;
        float z_paint = -cx * cx - cy * cy - cz * cz;

        if(object.background==true) {
  z_paint = z_paint - 1000;
}
face[6] = z_paint;
if((p1z>z_max || p2z>z_max || p3z>z_max)){
  if(p1z<z_clip && p2z<z_clip && p3z<z_clip){
    int s1x,s1y,s2x,s2y,s3x,s3y;
    s1x = p1[4]; s1y = p1[5];
    s2x = p2[4]; s2y = p2[5];
    s3x = p3[4]; s3y = p3[5];

    if(max(s3x,max(s1x,s2x))>0 && min(s3x,min(s1x,s2x))<128){
      if(( (s1x-s2x)*(s3y-s2y)-(s1y-s2y)*(s3x-s2x)) < 0){
        if(object.color_mode==k_colorize_dynamic){
          p2x = p2x - p1x;
          p2y = p2y - p1y;
          p2z = p2z - p1z;
          p3x = p3x - p1x;
          p3y = p3y - p1y;
          p3z = p3z - p1z;

          int nx = p2y*p3z-p2z*p3y;
          int ny = p2z*p3x-p2x*p3z;
          int nz = p2x*p3y-p2y*p3x;

          nx = nx<<2; ny = ny<<2; nz = nz<<2;
          double inv_dist = 1/sqrt(nx*nx+ny*ny+nz*nz);
          nx = nx*inv_dist;
          ny = ny*inv_dist;
          nz = nz*inv_dist;

          double mid_value = mid( nx*t_light_x+ny*t_light_y+nz*t_light_z,0,1)*(1-k_ambient)+k_ambient;
          double shade[2];
          color_shade(object.color, mid_value, shade);
          face[4] = shade[0];
          face[5] = shade[1];
        }

        triangle_list.push_back({s1x, s1y, s2x, s2y, s3x, s3y, z_paint, face[k_color1], face[k_color2]});
      }
    }
    else if(p1z < z_clip || p2z < z_clip || p3z < z_clip) {
    p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z = three_point_sort(p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z);
    if(p1z<z_clip && p2z<z_clip){
        float n2x,n2y,n2z;
        float n3x,n3y,n3z;
        std::tie(n2x,n2y,n2z) = z_clip_line(p2x,p2y,p2z,p3x,p3y,p3z,z_clip);
        std::tie(n3x,n3y,n3z) = za_clip_line(p3x,p3y,p3z,p1x,p1y,p1z,z_clip);

        float s1x,s1y;
        float s2x,s2y;
        float s3x,s3y;
        float s4x,s4y;
        std::tie(s1x,s1y) = project_point(p1x,p1y,p1z);
        std::tie(s2x,s2y) = project_point(p2x,p2y,p2z);
        std::tie(s3x,s3y) = project_point(n2x,n2y,n2z);
        std::tie(s4x,s4y) = project_point(n3x,n3y,n3z);

        if( max(s4x,max(s1x,s2x))>0 && min(s4x,min(s1x,s2x))<128)  {
            new_triangle({s1x,s1y,0}, {s2x,s2y,0}, {s4x,s4y,0}, z_paint, face[k_color1], face[k_color2});
        }
        if( max(s4x,max(s3x,s2x))>0 && min(s4x,min(s3x,s2x))<128)  {
            new_triangle({s2x,s2y,0}, {s4x,s4y,0}, {s3x,s3y,0}, z_paint, face[k_color1], face[k_color2]);
        }
    }else{
        float n1x,n1y,n1z;
        float n2x,n2y,n2z;
        std::tie(n1x,n1y,n1z) = z_clip_line(p1x,p1y,p1z,p2x,p2y,p2z,z_clip);
        std::tie(n2x,n2y,n2z) = z_clip_line(p1x,p1y,p1z,p3x,p3y,p3z,z_clip);

        float s1x,s1y;
        float s2x,s2y;
        float s3x,s3y;
        std::tie(s1x,s1y) = project_point(p1x,p1y,p1z);
        std::tie(s2x,s2y) = project_point(n1x,n1y,n1z);
        std::tie(s3x,s3y) = project_point(n2x,n2y,n2z);

        // Check if the triangle is visible and add to the list of visible triangles
        if (std::max(s3x, std::max(s1x, s2x)) > 0 && std::min(s3x, std::min(s1x, s2x)) < 128) {
            add(triangle_list, {s1x, s1y, s2x, s2y, s3x, s3y, z_paint, face[k_color1], face[k_color2]});
        }
    }
  }
}
    }
}

void draw_3d() {
    std::vector<Triangle> triangle_list;
    quicksort(object_list);
    start_timer();
    for (auto& object : object_list) {
        if (object.visible && !object.background) {
            render_object(object); // sort_faces(object);
            // if (object.color_mode == k_colorize_dynamic || object.color_mode == k_multi_color_dynamic) color_faces(object, object.color);
        }
    }
    render_time = stop_timer();
    start_timer();
    quicksort(triangle_list);
    sort_time = stop_timer();
    start_timer();
    draw_triangle_list(triangle_list);
    triangle_time = stop_timer();
}

