"""

 Examples of using pyrender for viewing and offscreen rendering.

"""


import os
import numpy as np
import trimesh
import math
import time

#==============================================================================
# Mesh creation
#==============================================================================

def prepareTeeth( modelfile, debug = False ):
    #stl파일같은 객체의 경로를 입력 받아 파이썬에서 쓸 수 있도록 바꿈, debug이 True이면 디버깅

   
    # 1. Creating textured meshes from trimeshes
    mesh = trimesh.load(modelfile) #'./lowerjaw.stl')
    '''
    Parameters
    file_obj (str, or file- like object) – 입력경로

    file_type (str) – 입력타입(ex:stl)

    resolver (trimesh.visual.Resolver) – 다른 객체나 텍스처를 참조 위해 로드

    force (None or str) – 'mesh'가 입력된 경우=>scene을 단일 mesh로, 'scene'의 경우=> 모든걸 scene으로 적용

    kwargs (dict) – geometry __init__로 전달됨

    Returns
    geometry – geometry를 trimesh classe로 로드
    '''
    if debug:
        print(mesh.__dict__.keys())
        # vertex 
        print(type(mesh.vertices), type(mesh.vertices).__bases__, mesh.vertices.shape)
        print("x:",np.min(mesh.vertices[:,0]), np.max(mesh.vertices[:,0])) 
        print("y:",np.min(mesh.vertices[:,1]), np.max(mesh.vertices[:,1])) 
        print("z:",np.min(mesh.vertices[:,2]), np.max(mesh.vertices[:,2])) 
     
        # faces 
        print(type(mesh.faces), len(mesh.faces))
        # visual colors
        print(mesh.visual)
        print(mesh.visual.defined, mesh.visual.kind)
        print(type(mesh.visual.face_colors))
        print(type(mesh.visual.face_colors).__bases__)
        print(mesh.visual.face_colors.shape)  # [300556,4)


    adjFaces = trimesh.graph.face_adjacency(mesh.faces, return_edges = False)#c++에서 
    #faces((n, 3) int, or None):삼각형을 구성하는 꼭짓점의 인덱스
    #mesh(Trimesh object):사용되면 face대신 edge가 사용됨
    #return edge(bool): edge를 return받을지 여부

    #return
    #adjacency((m,2)int): 인접한 faces(면?)의 인덱스를 반환
    #edges((m,2)int): 인접한 면들이 공유하는 가장자리를 반환
    
    #print(adjFaces.shape, edges.shape)
    #print(adjFaces[:5,:])
    #print(edges[:5,:])

    nFaces = len(mesh.faces)#mesh의 face 갯수
    adjFaceMap = np.zeros((nFaces, 4), dtype ="int")#adjFaceMap의 캔버스
    
    for i, (f1, f2) in enumerate(adjFaces):#인덱스와 원소 반환
        count = adjFaceMap[f1, 3]    # f1  
        adjFaceMap[f1,count] = f2
        adjFaceMap[f1,3] = count + 1
        
        count = adjFaceMap[f2, 3]    # f2 
        adjFaceMap[f2,count] = f1
        adjFaceMap[f2,3] = count + 1

    # check all has 3 faces 
    for i in range(nFaces):
        if adjFaceMap[i, 3] != 3:
          print("not 3 : ", i, adjFaceMap[i, 3])

    print(" all done ")

    return mesh, adjFaceMap 



def display(trimesh, labels, debug = False):
   
    """ Display suing Pyrender """

    import pyglet#opengl로 그림그리는 라이브러리
    pyglet.options['shadow_window'] = False
    
    from pyrender import PerspectiveCamera,\
                         DirectionalLight, SpotLight, PointLight,\
                         Mesh, Node, Scene, Viewer
   
    colors = [ (255,179,179), # marker 
               (255, 0 , 0),  (0, 255, 0), (0, 0, 255),
               (255, 0 , 0),  (0, 255, 0), (0, 0, 255),
               (255, 0 , 0),  (0, 255, 0), (0, 0, 255),
               (255, 0 , 0),  (0, 255, 0), (0, 0, 255),
               (255, 0 , 0),  (0, 255, 0), (0, 0, 255),
               (255, 0 , 0) ] 
   
    # coloring 모르겠당 ㅎㅎ 각 면을 색칠하는 것 같음
    trimesh.visual.face_colors[labels == -1,:3] = (255,255,255) #(128,128,128)  # undefined 
    #마킹 적용인듯
    for lab in range(len(colors)): #- 15):
        trimesh.visual.face_colors[labels == lab,:3] = colors[lab]#labels=lab인 지점의 0번째 인덱스 포함한 우측 3개 원소가 colors가 되도록
    #trimesh.visual.face_colors[labels == 1,:3] =  colors[1]
        
        
        
    if debug:
        print(trimesh.visual)
        print(trimesh.visual.defined, trimesh.visual.kind)
        
       
    #print(trimesh.visual.face_colors)
    #print(trimesh.faces,shape)
    teechMesh = Mesh.from_trimesh(trimesh, smooth = False)
    #trimesh에서 직접 triangular mesh model을 만들 수 있다.

    rotZ = -np.pi/2.0
    teethPose = np.array([
        [math.cos(rotZ), -math.sin(rotZ),  0.0, -0.0],
        [math.sin(rotZ),  math.cos(rotZ),  0.0,  0.0],
        [           0.0,             0.0,  1.0,  0.0],
        [           0.0,             0.0,  0.0,  1.0]
    ])
    #잘 모르겠당 ㅎㅎ
    #==============================================================================
    # 2. Light creation
    #==============================================================================
    #direc_l = (color=np.ones(3), intensity=1000.0)#진짜 뭐지...pyrender,무한 평면(거리에 따른 세기 변화 없음)
    #spot_l = SpotLight(color=np.ones(3), intensity=100.0,
    #                       innerConeAngle=np.pi/16, outerConeAngle=np.pi/6)#원뿔
    point_l1 = PointLight(color=np.ones(3), intensity=1000.0)#점(거리 제곱에 반비례),모든 방향으로 퍼짐
    point_l2 = PointLight(color=np.ones(3), intensity=1000.0)
    point_l3 = PointLight(color=np.ones(3), intensity=1000.0)
    point_l4 = PointLight(color=np.ones(3), intensity=1000.0)
        
    # 3. Camera creation
    cam = PerspectiveCamera(yfov=(np.pi / 3.0))#여전히 모르겠음 카메라 설정인듯
    cam_pose = np.array([
        [0.0,  -np.sqrt(2)/2, np.sqrt(2)/2, 0.5],
        [1.0, 0.0,           0.0,           0.0],
        [0.0,  np.sqrt(2)/2,  np.sqrt(2)/2, 0.4],
        [0.0,  0.0,           0.0,          1.0]
    ])

    # 4. Scene creation
    brigtness = 0.5
    scene = Scene(ambient_light=np.array([brigtness, brigtness, brigtness, 0.0]))#ambient_light:어디서나 빛이
                  #  bg_color =np.array([0, 0, 0, 1.0]))

    #scene.add(point_l)
    lightPose1 = np.array([
        [1.0, 0.0 , 0.0,  0.0],
        [0.0, 1.0,  0.0,  0.0],
        [0.0, 0.0,  1.0,  0.0],
        [0.0, 0.0,  0.0,  1.0]
    ])
    scene.add(point_l1, pose = lightPose1 )
    lightPose2 = np.array([
        [1.0, 0.0 , 0.0,  0.0],
        [0.0, 1.0,  0.0,  45.0],
        [0.0, 0.0,  1.0,  0.0],
        [0.0, 0.0,  0.0,  1.0]
    ])
    #scene.add(point_l2, pose = lightPose2 )
    lightPose3 = np.array([
        [1.0, 0.0 , 0.0,  30.0],
        [0.0, 1.0,  0.0,  30.0],
        [0.0, 0.0,  1.0,  0.0],
        [0.0, 0.0,  0.0,  1.0]
    ])
    scene.add(point_l3, pose = lightPose3 )
    lightPose4 = np.array([
        [1.0, 0.0 , 0.0,  -30.0],
        [0.0, 1.0,  0.0,  30.0],
        [0.0, 0.0,  1.0,  0.0],
        [0.0, 0.0,  0.0,  1.0]
    ])
    scene.add(point_l4, pose = lightPose4 )
    #scene.add(spot_l)
    #scene.add(direc_l)

    #fuze_node = Node(mesh=fuze_mesh, translation=np.array([0.1, 0.15, -np.min(fuze_trimesh.vertices[:,2])]))
    #scene.add_node(fuze_node)

    #------------------------------------------------------------------------------
    # By using the add() utility function
    #------------------------------------------------------------------------------
    teechNode = scene.add(teechMesh, pose = teethPose)
    #upperTeechNode = scene.add(upperTeechMesh, pose = upperTeethPose )

    #camNode = scene.add(cam, pose=cam_pose)

    #==============================================================================
    # Using the viewer with a default camera
    #============================================================================== 
    print("Creating Viewer")
    v = Viewer(scene, shadows=True)
    print("exited Viewer")



class candidate:#sorted list 위해 candidate타입 설정

    def __init__(self, faceId, marker, dist):
        self.faceId = faceId
        self.marker = marker
        self.dist   =  dist

    def __lt__(self, other):
        return self.dist < other.dist
        
    def __str__(self):
        #print(self.marker, self.dist, self.x, self,y)
        return "m=" +  str(self.marker) + ',d=' + str(self.dist)  + " at "  + str(self.faceId)   

#print(candidate(123, 100, 2)) 


# add the neighbors 무언가가 추가되었다...모르겠당
def addAdjFaces(S, marker, faceId, tag, adjFaceMap, faceNormals):
  
    for i in range(adjFaceMap[faceId,3]):#adjFaceMap이 주변부 3면을 말하는듯
        adjFaceId = adjFaceMap[faceId, i]
        if tag[adjFaceId] == -1:
            n1 = faceNormals[faceId]#라벨링
            n2 = faceNormals[adjFaceId]#???
            similarity = n1.dot(n2)#이게 뭘까..
            dist = - similarity  #  math.acos()#grad비슷한거인듯
            S.add (candidate(adjFaceId, marker, dist))

def initMarkers(nFaces, adjFaceMap, face_normals, debug = False):

    from sortedcontainers import SortedList
 
    # marker Initialization 
    labels = -1*np.ones(nFaces, dtype= 'int')  #  -1 for undefined yet

    S = SortedList()

    markerFaceIds = [ [139578, 109088, 140948, 293570, 167952, 171596, 149356, 142295, 148921, 150533, 153958, 165360, 168892], # 0
                      [43069, 213914, 205570], # 1 
                     [38798, 31806, 201908],  # 2
                     [28649],  # 3
                     [189128, 11920], # 4
                     [13517, 22288, 189364],  # 5
                     [6428, 184928],   # 6   
                     [4979, 184553],   # 7   
                     [4680, 8392],   # 8   
                      [9162, 7815],  # 9 
                      [6945, 5465],  # 10
                      [8737],  # 11
                      [27690, 14900], # 12
                      [22104, 17610, 189301], # 13
                      [15941, 19628], # 14
                      [39314], # 15
                      [210757] # 16
                      ]

    
    # seed marker 
    # nFaces = 300566
    
    for i in range(len(markerFaceIds)):#라벨에 넣음  
        for fid in markerFaceIds[i]:
            labels[fid] = i
     
    for i in range(len(markerFaceIds)):#sortedlist에 넣음   
        for fid in markerFaceIds[i]:
            addAdjFaces(S, i, fid,  labels, adjFaceMap, face_normals)      
        
    if debug:
        for i in range(len(S)):
            c = S[i] 
            print(i+1, c)

    return labels, S, len(markerFaceIds)
    
def segmentTeeth(mesh, adjFaceMap, showProgress = False):

    """
      IN : mesh face info + adjecent Faces info 
      OUT: labels (i.e., segmentation)
      
    """

    nFaces = len(mesh.faces)
    
    labels, S, nMarkers = initMarkers(nFaces, adjFaceMap, mesh.face_normals)

    if showProgress:
        display(mesh, labels)   #과정 보여줌
    
    nRemain = nFaces - nMarkers  # only seed are tagged at init 남은거
    
    while  nRemain > 0:#남은게 없을 때까지
        # 1. take the smallest distance from S
        if len(S) < 1:
            print(" S is empty!")
            break
        
        c = S.pop(0)  # 0 smallest, -1 largest 
        faceId = c.faceId
        marker = c.marker
        # 2. decide the tage for it and  recrease counter
        if labels[faceId] == -1: 
            labels[faceId] = marker
            nRemain  = nRemain - 1  
            
            if  nRemain % 50000 == 0:#50000개 단위로 디버깅(?)
                print("remain:", nRemain, "len(S):", len(S))
                if showProgress:#과정 보여줌
                    display(mesh, labels)                     #  dispaly results 
 
                #print("decide: f ", faceId,  "to m ", marker, "remain:", nRemain, "len(S):", len(S)) 
            #print(tag)  # 디버깅
            # 3. add the neihbors into S, (if it it not already done)
            addAdjFaces(S, marker, faceId,  labels, adjFaceMap, mesh.face_normals)  # adding nb for bg

    # check if S is empty or not 
    print("label-unique", np.unique(labels, return_counts = True))
    print("len(S):", len(S))     
    
    return labels  # labels 

if __name__ == '__main__':#메인함수..?

    mesh, adjFaceMap = prepareTeeth( './lowerjaw.stl') # load and prepare data structures
    
    t_s = time.time()
    labels = segmentTeeth(mesh, adjFaceMap, showProgress = False)     # run segmentation algorithm
    print("elapsed:", time.time() - t_s, "s")

    display(mesh, labels)                     #  dispaly results 
 
 
 

    
