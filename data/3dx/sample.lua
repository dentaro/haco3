object_list={}
obstacle_list={}
particle_list={}

k_color1=4
k_color2=5

k_screen_scale=80
k_x_center=64
k_y_center=64

z_clip=-3
z_max=-50

k_min_x=0
k_max_x=128
k_min_y=0
k_max_y=128

double_color_list= {{0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0},

                    {0,0,1,1,1,1,13,13,12,12},
                    {0,0,0,1,1,1,1,13,13,12},
                   
                    {2,2,2,2,8,8,14,14,14,15},
                    {0,1,1,2,2,8,8,8,14,14},
                   
                    {1,1,1,1,3,3,11,11,10,10},
                    {0,1,1,1,1,3,3,11,11,10},
                   
                    {1,1,2,2,4,4,9,9,10,10},
                    {0,1,1,2,2,4,4,9,9,10},
                   
                    {0,0,1,1,5,5,13,13,6,6},
                    {0,0,0,1,1,5,5,13,13,6},
                   
                    {1,1,5,5,6,6,6,6,7,7},
                    {0,1,1,5,5,6,6,6,6,7},
                   
                    {5,5,6,6,7,7,7,7,7,7},
                    {0,5,5,6,6,7,7,7,7,7},
                   
                    {2,2,2,2,8,8,14,14,15,15},
                    {0,2,2,2,2,8,8,14,14,15},
                   
                    {2,2,4,4,9,9,15,15,7,7},
                    {0,2,2,4,4,9,9,15,15,7},
                   
                    {4,4,9,9,10,10,7,7,7,7},
                    {0,4,4,9,9,10,10,7,7,7},
                   
                    {1,1,3,3,11,11,10,10,7,7},
                    {0,1,1,3,3,11,11,10,10,7},
                   
                    {13,13,13,12,12,12,6,6,7,7},
                    {0,5,13,13,12,12,12,6,6,7},
                   
                    {1,1,5,5,13,13,6,6,7,7},
                    {0,1,1,5,5,13,13,6,6,7},
                   
                    {2,2,2,2,14,14,15,15,7,7},
                    {0,2,2,2,2,14,14,15,15,7},
                   
                    {4,4,9,9,15,15,7,7,7,7},
                    {0,4,4,9,9,15,15,7,7,7}
                    }

function camera(x, y)
  if x ~= nil then
    cam_x = x
  end
  if y ~= nil then
    cam_y = y
  end
end

function add(t, v)
  t[#t+1] = v
end

function del(t, v)
  for i=#t,1,-1 do
  if t[i] == v then table.remove(t, i) end
  end
end

function foreach(t, f)
  for _, v in ipairs(t) do
    f(v)
  end
end


function new_object()
  object={}
  object.vertices={}
  object.faces={}
 
  object.t_vertices={}

  object.x=0
  object.y=0
  object.z=0
 
  object.rx=0
  object.ry=0
  object.rz=0
 
  object.tx=0
  object.ty=0
  object.tz=0
 
  object.ax=0
  object.ay=0
  object.az=0
 
  object.sx=0
  object.sy=0
  object.radius=10
  object.sradius=10
  object.visible=true
 
  object.render=true
  object.background=false
  object.collision_x=true
  object.collision_y=false
  object.collision_down=false
  object.collision_up=false
  object.collision_left=false
  object.ring=false
 
  object.min_x=100
  object.min_y=100
  object.min_z=100
 
  object.max_x=-100
  object.max_y=-100
  object.max_z=-100
 
  object.vx=0
  object.vy=0
  object.vz=0

  object.age=0
  object.health=2
  add(object_list,object)
  return object

end

-- init_player()
player=new_object()
player.min_x=-4.5
player.min_y=-4.5
player.min_z=-4.5
player.max_x=4.5
player.max_y=4.5
player.max_z=4.5

player.x=0
player.y=8
player.z=15

player.vx=0
player.vy=0
player.vz=0

-- init_light()
light1_x=0.1
light1_y=0.35
light1_z=0.2
t_light_x=0
t_light_y=0
t_light_z=0

function normalize(x,y,z)
  local x1=shl(x,2)
  local y1=shl(y,2)
  local z1=shl(z,2)
 
  local inv_dist=1/sqrt(x1*x1+y1*y1+z1*z1)
 
  return x1*inv_dist,y1*inv_dist,z1*inv_dist
 
end


light1_x,light1_y,light1_z=normalize(light1_x,light1_y,light1_z)

function intersect_bounding_box(object_a, object_b)
  return 
      ((object_a.min_x+object_a.x < object_b.max_x+object_b.x) and (object_a.max_x+object_a.x > object_b.min_x+object_b.x) and
       (object_a.min_y+object_a.y < object_b.max_y+object_b.y) and (object_a.max_y+object_a.y > object_b.min_y+object_b.y) and
       (object_a.min_z+object_a.z < object_b.max_z+object_b.z) and (object_a.max_z+object_a.z > object_b.min_z+object_b.z))
end

k_friction=0.7
function update_player()
   
    old_x=player.x
    old_y=player.y
    old_z=player.z

    player.y=player.y+player.vy

    player.x=player.x+player.vx

    for _, object in pairs(obstacle_list) do
        if( intersect_bounding_box(player, object)) then player.vx=0 player.x=old_x end
    end
    player.z=player.z+player.vz
    for _, object in pairs(obstacle_list) do
        if( intersect_bounding_box(player, object)) then player.vz=0 player.z=old_z end
    end
    player.vx=player.vx*k_friction
    player.vy=player.vy*k_friction
    player.vz=player.vz*k_friction
end

function generate_cam_matrix_transform(xa,ya,za)
  local sx=sin(xa)
  local sy=sin(ya)
  local sz=sin(za)
  local cx=cos(xa)
  local cy=cos(ya)
  local cz=cos(za)
 
  cam_mat00=cz*cy
  cam_mat10=-sz
  cam_mat20=cz*sy
  cam_mat01=cx*sz*cy+sx*sy
  cam_mat11=cx*cz
  cam_mat21=cx*sz*sy-sx*cy
  cam_mat02=sx*sz*cy-cx*sy
  cam_mat12=sx*cz
  cam_mat22=sx*sz*sy+cx*cy
end


function update_camera()   
  cam_x=player.x
  cam_y=player.y
  cam_z=player.z

  cam_ax=player.ax
  cam_ay=player.ay
  cam_az=player.az

  generate_cam_matrix_transform(cam_ax,cam_ay,cam_az)
end

function generate_matrix_transform(xa,ya,za)
  local sx=sin(xa)
  local sy=sin(ya)
  local sz=sin(za)
  local cx=cos(xa)
  local cy=cos(ya)
  local cz=cos(za)
  mat00=cz*cy
  mat10=-sz
  mat20=cz*sy
  mat01=cx*sz*cy+sx*sy
  mat11=cx*cz
  mat21=cx*sz*sy-sx*cy
  mat02=sx*sz*cy-cx*sy
  mat12=sx*cz
  mat22=sx*sz*sy+cx*cy
end

function rotate_point(x,y,z)   
  return (x)*mat00+(y)*mat10+(z)*mat20,(x)*mat01+(y)*mat11+(z)*mat21,(x)*mat02+(y)*mat12+(z)*mat22
end


function set_radius(object)
  for _, vertex in pairs(object.vertices) do
      object.radius=max(object.radius,vertex[1]*vertex[1]+vertex[2]*vertex[2]+vertex[3]*vertex[3])
  end
  object.radius=sqrt(object.radius)
end

function set_bounding_box(object)
  for _, vertex in pairs(object.vertices) do
      object.min_x=min(vertex[1],object.min_x)
      object.min_y=min(vertex[2],object.min_y)
      object.min_z=min(vertex[3],object.min_z)
      object.max_x=max(vertex[1],object.max_x)
      object.max_y=max(vertex[2],object.max_y)
      object.max_z=max(vertex[3],object.max_z)
  end
end

function transform_object(object)
  if(object.visible)then
      generate_matrix_transform(object.ax,object.ay,object.az)
      for i=1, #object.vertices do
          local t_vertex=object.t_vertices[i]
          local vertex=object.vertices[i]
          t_vertex[1],t_vertex[2],t_vertex[3]=rotate_point(vertex[1],vertex[2],vertex[3])
      end
  end
end

function vector_cross_3d(px,py,pz,ax,ay,az,bx,by,bz)
  ax = ax - px
  ay = ay - py
  az = az - pz
  bx = bx - px
  by = by - py
  bz = bz - pz
  local dx=ay*bz-az*by
  local dy=az*bx-ax*bz
  local dz=ax*by-ay*bx
  return dx,dy,dz
end

function vector_dot_3d(ax,ay,az,bx,by,bz)
  return ax*bx+ay*by+az*bz
end

function color_shade(color,brightness)
  local b= band(brightness*10,0xffff)
  local c= (color+1)*2
  return double_color_list[ c-1 ][b] , double_color_list[ c ][b] 
end       

k_ambient = 0.3
function color_faces(object,base)
        for i=1,#object.faces do
            local face=object.faces[i]
            local p1x=object.t_vertices[face[1]][1]
            local p1y=object.t_vertices[face[1]][2]
            local p1z=object.t_vertices[face[1]][3]
            local p2x=object.t_vertices[face[2]][1]
            local p2y=object.t_vertices[face[2]][2]
            local p2z=object.t_vertices[face[2]][3]
            local p3x=object.t_vertices[face[3]][1]
            local p3y=object.t_vertices[face[3]][2]
            local p3z=object.t_vertices[face[3]][3]       
            local nx,ny,nz = vector_cross_3d(p1x,p1y,p1z,
                                p2x,p2y,p2z,
                                p3x,p3y,p3z)
        nx,ny,nz = normalize(nx,ny,nz)
        local b = vector_dot_3d(nx,ny,nz,light1_x,light1_y,light1_z)

        if(object.color_mode==k_multi_color_dynamic)then
            face[4],face[5]=color_shade(object.base_faces[i][4], mid( b,0,1)*(1-k_ambient)+k_ambient )
        else
            face[4],face[5]=color_shade(base, mid( b,0,1)*(1-k_ambient)+k_ambient )
        end
    end
end

k_colorize_static = 1
k_colorize_dynamic = 2
k_multi_color_static = 3
k_multi_color_dynamic = 4
k_preset_color = 5

function load_object(object_vertices,object_faces,x,y,z,ax,ay,az,obstacle,color_mode,color)
  object=new_object()
  object.vertices=object_vertices
  if(color_mode==k_preset_color)then
      object.faces=object_faces
  else
      object.base_faces=object_faces
      object.faces={}
      for i=1,#object_faces do
          object.faces[i]={}
          for j=1,#object_faces[i] do
              object.faces[i][j]=object_faces[i][j]
          end
      end
  end

  object.radius=0
  for i=1,#object_vertices do
      object.t_vertices[i]={}
          for j=1,3 do
              object.t_vertices[i][j]=object.vertices[i][j]
          end
  end

  object.ax=ax or 0
  object.ay=ay or 0
  object.az=az or 0
 
  transform_object(object)
 
  set_radius(object)
  set_bounding_box(object)
 
  object.x=x or 0
  object.y=y or 0
  object.z=z or 0
 
  object.color = color or 8
  object.color_mode= color_mode or k_colorize_static
 
  object.obstacle = obstacle or false
 
  if(obstacle)then
    add(obstacle_list,object)
  end
 
  if(color_mode==k_colorize_static or color_mode==k_colorize_dynamic or color_mode==k_multi_color_static )then
      color_faces(object,color)
  end
 
  return object
end

function delete_object(object)
  del(object_list,object)
end

---------------------------------

function rotate_cam_point(x,y,z)
  return (x)*cam_mat00+(y)*cam_mat10+(z)*cam_mat20,(x)*cam_mat01+(y)*cam_mat11+(z)*cam_mat21,(x)*cam_mat02+(y)*cam_mat12+(z)*cam_mat22
end

function project_point(x,y,z)
  return x*k_screen_scale/z+k_x_center,y*k_screen_scale/z+k_x_center
end

function project_radius(r,z)
  return r*k_screen_scale/abs(z)
end

function is_visible(object)
  if(object.tz+object.radius>z_max and object.tz-object.radius<z_clip and
     object.sx+object.sradius>0 and object.sx-object.sradius<128 and
     object.sy+object.sradius>0 and object.sy-object.sradius<128 )
     then return true else return false end
end

function update_visible(object)
  object.visible=false
  local px,py,pz = object.x-cam_x,object.y-cam_y,object.z-cam_z
  object.tx, object.ty, object.tz =rotate_cam_point(px,py,pz)
  object.sx,object.sy = project_point(object.tx,object.ty,object.tz)
  object.sradius=project_radius(object.radius,object.tz)
  object.visible= is_visible(object)
end

function cam_transform_object(object)
  if(object.visible)then
      for i=1, #object.vertices do
          local vertex=object.t_vertices[i]
          vertex[1]=vertex[1]+(object.x - cam_x)
          vertex[2]=vertex[2]+(object.y - cam_y)
          vertex[3]=vertex[3]+(object.z - cam_z)
          vertex[1],vertex[2],vertex[3]=rotate_cam_point(vertex[1],vertex[2],vertex[3])
      end
  end
end

function update_light()
  t_light_x,t_light_y,t_light_z = rotate_cam_point(light1_x,light1_y,light1_z)
end

function update_3d()
  for _, object in pairs(object_list) do
          update_visible(object)
          transform_object(object)
          cam_transform_object(object)
          update_light()
  end
end


-------------------------
function quicksort(t, start, endi)
   start, endi = start or 1, endi or #t
  if(endi - start < 1) then return t end
  local pivot = start
  for i = start + 1, endi do
    if t[i].tz <= t[pivot].tz then
      if i == pivot + 1 then
        t[pivot],t[pivot+1] = t[pivot+1],t[pivot]
      else
        t[pivot],t[pivot+1],t[i] = t[i],t[pivot],t[pivot+1]
      end
      pivot = pivot + 1
    end
  end
   t = quicksort(t, start, pivot - 1)
  return quicksort(t, pivot + 1, endi)
end

local timer_value

function start_timer()
  timer_value = os.clock()
end

function stop_timer()
  return (os.clock() - timer_value) * 1000
end



function shade_trifill( x1,y1,x2,y2,x3,y3, color1, color2)
    local x1=band(x1,0xffff)
    local x2=band(x2,0xffff)
    local y1=band(y1,0xffff)
    local y2=band(y2,0xffff)
    local x3=band(x3,0xffff)
    local y3=band(y3,0xffff)
    
    local nsx,nex

    if(y1>y2)then
      y1,y2=y2,y1
      x1,x2=x2,x1
    end
    
    if(y1>y3)then
      y1,y3=y3,y1
      x1,x3=x3,x1
    end
    
    if(y2>y3)then
      y2,y3=y3,y2
      x2,x3=x3,x2          
    end

    if(y1 ~= y2)then
      local delta_sx=(x3-x1)/(y3-y1)
      local delta_ex=(x2-x1)/(y2-y1)
     
      if(y1>0)then
          nsx=x1
          nex=x1
          min_y=y1
      else --top edge clip
          nsx=x1-delta_sx*y1
          nex=x1-delta_ex*y1
          min_y=0
      end
     
      max_y=min(y2,128)
     
      for y=min_y,max_y-1 do

      --rectfill(nsx,y,nex,y,color1)
      if(band(y,1)==0)then rectfill(nsx,y,nex,y,color1) else rectfill(nsx,y,nex,y,color2) end
      nsx=nsx+delta_sx
      nex=nex+delta_ex
      end

  else --where top edge is horizontal
      nsx=x1
      nex=x2
  end

    
  if(y3~=y2)then
      local delta_sx=(x3-x1)/(y3-y1)
      local delta_ex=(x3-x2)/(y3-y2)
     
      min_y=y2
      max_y=min(y3,128)
      if(y2<0)then
          nex=x2-delta_ex*y2
          nsx=x1-delta_sx*y1
          min_y=0
      end
     
       for y=min_y,max_y do

          --rectfill(nsx,y,nex,y,color1)
          if(band(y,1)==0)then rectfill(nsx,y,nex,y,color1) else rectfill(nsx,y,nex,y,color2) end
          nex=nex+delta_ex
          nsx=nsx+delta_sx
       end
     
  else --where bottom edge is horizontal
      --rectfill(nsx,y3,nex,y3,color1)
      if(band(y,1)==0)then rectfill(nsx,y3,nex,y3,color1) else rectfill(nsx,y3,nex,y3,color2) end
  end

end

function draw_triangle_list()
  for i=1,#triangle_list do
      local t=triangle_list[i]
      shade_trifill( t.p1x,t.p1y,t.p2x,t.p2y,t.p3x,t.p3y, t.c1,t.c2 )
  end
end

function three_point_sort(p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z)
  if(p1z>p2z) then p1z,p2z = p2z,p1z p1x,p2x = p2x,p1x p1y,p2y = p2y,p1y end
  if(p1z>p3z) then p1z,p3z = p3z,p1z p1x,p3x = p3x,p1x p1y,p3y = p3y,p1y end
  if(p2z>p3z) then p2z,p3z = p3z,p2z p2x,p3x = p3x,p2x p2y,p3y = p3y,p2y end
 
  return p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z
end

function lerp(a,b,alpha)
  return a*(1.0-alpha)+b*alpha
end

function z_clip_line(p1x,p1y,p1z,p2x,p2y,p2z,clip)
  if(p1z>p2z)then
      p1x,p2x=p2x,p1x
      p1z,p2z=p2z,p1z
      p1y,p2y=p2y,p1y
  end
  if(clip>p1z and clip<=p2z)then
      alpha= abs((p1z-clip)/(p2z-p1z))
      nx=lerp(p1x,p2x,alpha)
      ny=lerp(p1y,p2y,alpha)
      nz=lerp(p1z,p2z,alpha)
      return nx,ny,nz
  else
      return false
  end
end

function new_triangle(p1x,p1y,p2x,p2y,p3x,p3y,z,c1,c2)
  add(triangle_list,{p1x=p1x,
                     p1y=p1y,
                     p2x=p2x,
                     p2y=p2y,
                     p3x=p3x,
                     p3y=p3y,
                     tz=z,
                     c1=c1,
                     c2=c2})
end

function render_object(object)
      for i=1, #object.t_vertices do
          local vertex=object.t_vertices[i]
          vertex[4],vertex[5] = vertex[1]*k_screen_scale/vertex[3]+k_x_center,vertex[2]*k_screen_scale/vertex[3]+k_x_center
      end
      for i=1,#object.faces do
  
          local face=object.faces[i]
     
          local p1=object.t_vertices[face[1]]
          local p2=object.t_vertices[face[2]]
          local p3=object.t_vertices[face[3]]
         
          local p1x,p1y,p1z=p1[1],p1[2],p1[3]
          local p2x,p2y,p2z=p2[1],p2[2],p2[3]
          local p3x,p3y,p3z=p3[1],p3[2],p3[3]
  
          local cz=0.01*(p1z+p2z+p3z)/3
          local cx=0.01*(p1x+p2x+p3x)/3
          local cy=0.01*(p1y+p2y+p3y)/3
          local z_paint= -cx*cx-cy*cy-cz*cz

          if(object.background==true) then z_paint=z_paint-1000 end
          face[6]=z_paint
  
          if((p1z>z_max or p2z>z_max or p3z>z_max))then
              if(p1z< z_clip and p2z< z_clip and p3z< z_clip)then
  
                      local s1x,s1y = p1[4],p1[5]
                      local s2x,s2y = p2[4],p2[5]
                      local s3x,s3y = p3[4],p3[5]

                      if( max(s3x,max(s1x,s2x))>0 and min(s3x,min(s1x,s2x))<128)  then
                          if(( (s1x-s2x)*(s3y-s2y)-(s1y-s2y)*(s3x-s2x)) < 0)then
                              if(object.color_mode==k_colorize_dynamic)then
                                p2x = p2x - p1x
                                p2y = p2y - p1y
                                p2z = p2z - p1z
                                p3x = p3x - p1x
                                p3y = p3y - p1y
                                p3z = p3z - p1z
  
                                local nx = p2y*p3z-p2z*p3y
                                local ny = p2z*p3x-p2x*p3z
                                local nz = p2x*p3y-p2y*p3x
                                
                                nx=shl(nx,2) ny=shl(ny,2) nz=shl(nz,2)
                                local inv_dist=1/sqrt(nx*nx+ny*ny+nz*nz)
                                nx=nx*inv_dist 
                                ny=ny*inv_dist 
                                nz=nz*inv_dist                       
                                                            
                                face[4],face[5]=color_shade(object.color, mid( nx*t_light_x+ny*t_light_y+nz*t_light_z,0,1)*(1-k_ambient)+k_ambient )
                              end
  
                              add(triangle_list,{p1x=s1x,
                                                  p1y=s1y,
                                                  p2x=s2x,
                                                  p2y=s2y,
                                                  p3x=s3x,
                                                  p3y=s3y,
                                                  tz=z_paint,
                                                  c1=face[k_color1],
                                                  c2=face[k_color2]})

                          end
                      end
              elseif(p1z< z_clip or p2z< z_clip or p3z< z_clip)then
                  p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z = three_point_sort(p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z)
                  if(p1z<z_clip and p2z<z_clip)then
  
                      local n2x,n2y,n2z = z_clip_line(p2x,p2y,p2z,p3x,p3y,p3z,z_clip)
                      local n3x,n3y,n3z = z_clip_line(p3x,p3y,p3z,p1x,p1y,p1z,z_clip)
  
                      local s1x,s1y = project_point(p1x,p1y,p1z)
                      local s2x,s2y = project_point(p2x,p2y,p2z)
                      local s3x,s3y = project_point(n2x,n2y,n2z)
                      local s4x,s4y = project_point(n3x,n3y,n3z)

                      if( max(s4x,max(s1x,s2x))>0 and min(s4x,min(s1x,s2x))<128)  then
                          new_triangle(s1x,s1y,s2x,s2y,s4x,s4y,z_paint,face[k_color1],face[k_color2])
                      end
                      if( max(s4x,max(s3x,s2x))>0 and min(s4x,min(s3x,s2x))<128)  then
                          new_triangle(s2x,s2y,s4x,s4y,s3x,s3y,z_paint,face[k_color1],face[k_color2])
                      end
                  else

                      local n1x,n1y,n1z = z_clip_line(p1x,p1y,p1z,p2x,p2y,p2z,z_clip)
                      local n2x,n2y,n2z = z_clip_line(p1x,p1y,p1z,p3x,p3y,p3z,z_clip)

                      local s1x,s1y = project_point(p1x,p1y,p1z)
                      local s2x,s2y = project_point(n1x,n1y,n1z)
                      local s3x,s3y = project_point(n2x,n2y,n2z)
  
                      if( max(s3x,max(s1x,s2x))>0 and min(s3x,min(s1x,s2x))<128)  then
                          new_triangle(s1x,s1y,s2x,s2y,s3x,s3y,z_paint,face[k_color1],face[k_color2])
                      end
                  end
              end
          end
      end
  end

function draw_3d()
  triangle_list={}
  quicksort(object_list)
  start_timer()
  for _, object in pairs(object_list) do
      if(object.visible and not object.background) then
          render_object(object) --sort_faces(object)
          --if(object.color_mode==k_colorize_dynamic or object.color_mode==k_multi_color_dynamic) color_faces(object,object.color)
      end
  end
  render_time=stop_timer()
  start_timer()
      quicksort(triangle_list)
  sort_time=stop_timer()
  start_timer()
      draw_triangle_list()
  triangle_time=stop_timer()
end

----------------------------------END COPY-------------------------------------------------------
----------------------------------Electric Gryphon's 3D Library----------------------------------
-------------------------------------------------------------------------------------------------

period=24
frame=0
crash=0
cube_v={
  { 1.0, -1.0, -1.0},
  { 1.0, -1.0, 1.0},
  {-1.0, -1.0, 1.0},
  {-1.0, -1.0, -1.0},
  { 1.0, 1.0, -1.0},
  { 1.0, 1.0, 1.0},
  {-1.0, 1.0, 1.0},
  {-1.0, 1.0, -1.0}
  }
  
  cube_f={
  {1, 2, 3, 8,9},
  {3, 4, 1, 8,9},
  {5, 8, 7, 9,10},
  {7, 6, 5, 9,10},
  {1, 5, 6, 8,9},
  {6, 2, 1, 8,9},
  {2, 6, 7, 9,10},
  {7, 3, 2, 9,10},
  {3, 7, 8, 8,9},
  {8, 4, 3, 8,9},
  {5, 1, 4, 9,10},
  {4, 8, 5, 9,10}
  }
  
  ship_v={
  { 0.0, 0.5,-0.5},
  {-1.0, 0.0, 0.0},
  { 1.0, 0.0, 0.0},
  { 0.0, 0.0,-2.4},
  }
  
  ship_f={
  {2, 1, 4},
  {3, 4, 1},
  {4, 3, 2},
  {2, 3, 1}
  }
  
--   --立方体を配置
  function new_cube()
  if frame%period==1 then
  local x=4-rnd(3)*4
  local col=flr(rnd(8))+8
  local c=load_object(cube_v,cube_f,x,4,-20,0,0,0,false,k_colorize_dynamic,col)
  c.vx=0.0
  c.vy=0.0
  c.vz=0.5
  end
  end
  
  --立方体を更新
  function update_cube(c)
  --宇宙船は除外
  if c==ship then
  return
  end
  --視界から外れていったら削除する (残したままにしておくと、メモリがパンクします)
  if c.z>player.z or c.y>10.0 then
  delete_object(c)
  return
  end
  c.x = c.x + c.vx
  c.y = c.y + c.vy
  c.z = c.z + c.vz
  c.vz = c.vz + 0.05
  c.ax = c.ax + 0.01
  c.ay = c.ay + 0.01
  c.az = c.az + 0.01
  --衝突判定
  if intersect_bounding_box(c,ship) then
  -- sfx(0) --お好みの効果音をどうぞ
  crash=crash+8
  c.vz= c.vz - 1.5
  c.vx=rnd(1.0)-0.5
  c.vy=1.0
  end
  end
  
  --ボタン入力
  function input()
    if btn(1) then ship.x = ship.x + 0.2 end
    if btn(2) then ship.x = ship.x - 0.2 end
    if btn(3) then ship.z = ship.z - 0.2 end
    if btn(4) then ship.z = ship.z + 0.2 end
   end


--ぶつかった時、画面を揺らす
function tilt_screen()
  if crash>0 then
    camera(rnd(10)-5, rnd(10)-5)
    crash=crash-1
  else
    camera(0,0)
  end
end

ship=load_object(ship_v,ship_f,0,4,11,0.0,0,0,false,k_colorize_dynamic,13)
-- --視界の初期状態をちょっと調整
player.ax=player.ax-0.075
player.y=10

function _update()
  frame = frame + 1
  update_player()
  update_camera()
  input()
  new_cube()
  foreach(object_list, update_cube)
  tilt_screen()
  update_3d()
end
    
function _draw()
  rectfill(0,0,127,127,1)
  draw_3d()
end
-------------------------------------------------------
function setup()--init
end

function loop()--update --draw
_update()
_draw()
end
-------------------------------------------------------