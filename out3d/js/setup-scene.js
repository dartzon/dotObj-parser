var container, scene, camera, renderer, group;

var targetRotation = 0;
var targetRotationOnMouseDown = 0;

var mouseX = 0;
var mouseXOnMouseDown = 0;

var windowHalfX = window.innerWidth / 2;
var windowHalfY = window.innerHeight / 2;

init();
animate();

var arr = [];
arr[0] = new THREE.Vector3( -50, -50, 0 );
arr[1] = new THREE.Vector3(  50, -50, 0 );
arr[2] = new THREE.Vector3(  50,  50, 0 );

function init()
{
    container = document.createElement('div');
    document.body.appendChild(container);

    scene = new THREE.Scene();

    camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 1, 1000);
    scene.add(camera);

    var light = new THREE.PointLight(0xffffff, 2.2);
    camera.add(light);

    /* var light = new THREE.AmbientLight( 0x404040, 7 ); // soft white light*/
    /* scene.add( light );*/

    group = new THREE.Group();
    group.position.y = 50;
    scene.add(group);

    // =============================================================================================

    var material = new THREE.MeshStandardMaterial( { color : 0xc2c2c2 } );

    // Render the Wavefront Obj model.
    var geometry = renderObjFile();

    //the face normals and vertex normals can be calculated automatically if not supplied above
    geometry.computeFaceNormals();
    geometry.computeVertexNormals();

    var triangleMesh = new THREE.Mesh( geometry, material );

    var eGeometry = new THREE.EdgesGeometry( triangleMesh.geometry );
    var eMaterial = new THREE.LineBasicMaterial( { color: 0x000000, linewidth: 1 } );
    var edges = new THREE.LineSegments( eGeometry , eMaterial );

    triangleMesh.add(edges);
    group.add(triangleMesh);

    camera.position = group.position;
    camera.position.z += 150;

    // =============================================================================================

    renderer = new THREE.WebGLRenderer({
	antialias: true
    });
    renderer.setClearColor(0xcccccc);
    renderer.setPixelRatio(window.devicePixelRatio);
    renderer.setSize(window.innerWidth, window.innerHeight);
    container.appendChild(renderer.domElement);

    var controls = new THREE.OrbitControls(camera, renderer.domElement, renderer, scene);
}

function animate()
{
    camera.updateProjectionMatrix();
    requestAnimationFrame(animate);
    render();
}

function render()
{
    /* group.rotation.y += ( targetRotation - group.rotation.y ) * 0.05;*/
    renderer.render(scene, camera);
}
