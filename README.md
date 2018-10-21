<h1>Introduction</h1>
Hey there!

To start with – it isn’t a tutorial series. I wanted to start a blog to motivate myself even more to start and finish DirectX 11 project. If anyone would find it inspiring or will learn anything from me, I would  be really happy, even if that’s not the point of this blog.

Project itself is a remake of 2D Platformer game that I’ve made with my friend for Ludum Dare 36 https://azargaz.itch.io/ancient-drone. This time I’d like to make it from scratch using DirectX 11. I will create flipbook animation system, basic physics engine, enemies with simple AI and other things essential to complete this remake. Let’s get started!

<h2>Week #1</h2>
To begin our adventure I’ve prepared basic pipeline for rendering 2D images. Image is created simply by rendering rectangle (2 triangles) with specified size and color. In future I will implement textures (or even .png files) importing to allow using sprites. Base of my pipeline is based on Rasterek tutorial, up until #4 tutorial - http://www.rastertek.com/dx11tut04.html. I highly recommend his tutorial series to begin with DirectX 11 programming.

Second thing that I needed to start working on game was physics engine. Currently it’s horrible primitive but that’s enough for jumping on platforms (and so enough for PLATFORMer game). In each model instance I’ve added physics bounds. Each object has its bounds initialized on creation but they can be modified if we’d like for example enemies to have larger colliders or make player’s vulnerability collider smaller than model itself. Physics is based on simple comparisons in FixedUpdate() (I derived this name from Unity engine as an Update with fixed timestep):

<pre><code class="cpp">
bool isFalling = true;
bool isGround = false;

if (jumping)
{
	isFalling = false;
	//Add positive velocity
}
//We are calculating further only if player is standing on ground or is falling and thus can interact with platform in air
if (player.min.y - groundThickness <= ground.max.y)
{
	//Player standing on platform
	if (player.min.x < ground.max.x && player.max.x > ground.min.x)
	{
		isFalling = false;
		isGround = true;
	}
	else if (player.max.x < ground.max.x)
	{
		if (ground.min.x - player.max.x <= 0.0f && velocity > 0.0f)
			//Snap to left side of platform		
	}
	else if (player.min.x > ground.min.x)
	{
		if (-(player.min.x - ground.max.x) >= 0.0f && velocity < 0.0f)
			//Snap to right side of platform
	}
}

if (isFalling)
	//Apply gravity
</code></pre>

And that’s all. I’m simply testing 4 states: player jumping, player standing on ground, player moving inside platform from left side (snap to left), player moving inside platform from right side (snap to right). If player isn’t jumping and isn’t standing on ground then it must be falling so I apply gravity in that case scenario.

After every week progress I will post new release version with changes that I’ve made through that week. Thank you for reading and once again – Let the adventure begin!
