fn GetLevelName = (	return "D:\Game\Data\Level" )
fn GetCores = ( return $*)
fn GetInstances = (	return $* )
fn GetYaw transform = ( return -(transform as eulerangles).z )
fn GetPitch transform = ( return -(transform as eulerangles).y )
fn GetRoll transform = ( return (transform as eulerangles).x )
--fn GetTransformPos obj = ( return "%" obj.pos.x) 
function ExportStaticMeshes Objs=
(
	local file = createfile (GetLevelName() + "\\static_meshes.xml")
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<static_meshes>\n" to:file
	for i=1 to Objs.count do
	(
		format "\t<static_mesh name=\"%\" filename=\"%\">\n" Objs[i].name ("Data\\Level\\" + Objs[i].name) to:file
	)
	format "</static_meshes>\n" to:file
	close file
)	

function ExportRenderableObjects Objs=
(
	local file = createfile (GetLevelName() + "\\renderable_objects.xml")
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<renderable_objects>\n" to:file
	for i=1 to Objs.count do
	(
		format "\t<mesh_instance name=\"%\" core_name=\"%\" pos=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" visible=\"true\"/>\n" Objs[i].name Objs[i].name Objs[i].pos.x Objs[i].pos.y Objs[i].pos.z (GetYaw Objs[i].transform) (GetPitch Objs[i].transform) (GetRoll Objs[i].transform) to:file
	)
	format "</renderable_objects>\n" to:file
	close file
)

function ExportMaterials Obj file =
(
	WriteShort file Obj.material.count
	if (classof Obj.material == "Standardmaterial")
	(
		format Obj.material.diffuseMap.name 
	)
	else
	(
		for i =1 to Obj.material.count do
		(
			
		)
	)
)

function ExportMesh Obj=
(
	local file = createfile (GetLevelName() + Obj.name +  ".mesh")
	WriteShort file FE55 #unsigned
	ExportMaterials Obj file
	ExportMeshes Obj file
	WriteShort file 55FE #unsigned
)

ExportStaticMeshes $*
ExportRenderableObjects $*
ExportMeshes $