<a href="https://komilll.github.io/AncientDroneRemake/">Better formatting</a>

<a href="Blog/Post-mortem.pdf">Postmortem</a>

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/1.0">Final Release (.exe)</a>
<h1>Introduction</h1>
Hey there!

To start with – it isn’t a tutorial series. I wanted to start a blog to motivate myself even more to start and finish DirectX 11 project. If anyone would find it inspiring or will learn anything from me, I would  be really happy, even if that’s not the point of this blog.

Project itself is a remake of 2D Platformer game that I’ve made with my friend for Ludum Dare 36
<a href="https://azargaz.itch.io/ancient-drone">https://azargaz.itch.io/ancient-drone</a>. This time I’d like to make it from scratch using DirectX 11. I will create flipbook animation system, basic physics engine, enemies with simple AI and other things essential to complete this remake. Let’s get started!

<h2>Week #11 [Christmas week]</h2>
Polishing, testing, debugging. Also preparing release and postmortem.

<h2>Week #10</h2>
At week #9 I've finished everything gameplay related (even though I want to tweak some settings on last week #11). This week I've spent on creating particle system for dust that is appearing behind moving player. Other thing was parallax scrolling background and some minor fixes of bugs that I've noticed.
Parallax scrolling was made with 3 sprites that are moving with different speed. The farthest sky is the slowest and sand which is the nearest is slightly faster. I've already made for Models to follow Player so I only needed to connect backgrounds to it. Previously for example UI and Cursor was following the player every frame (because I am not using screenspace and everything is based on models).

Particle system is based on 4 similar sprites that differ by size. Additional every particle is resized randomly, shifted by random position and then alpha is blended over time to 0. No movement of particle is involved as player creates an illusion of motion because of his and camera movement.

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/0.10">https://github.com/komilll/AncientDroneRemake/releases/tag/0.10</a> - Week #10 release
<center><img src="Blog/gif_10.gif"></center>
<center><i>Week #10: Gif #10 – Parallax Scrolling</i></center>

<h2>Week #9</h2>
Everything related to the gameplay from original game is ready. What lacked was secondary drone attack (explosion) which can be used with Right Mouse Button. Another thing were traps (dark spheres) and changing levels.

Also some physics issues, jump physics upgrade and lerping player position instead of instantly jumping after being damaged. Engine was implemented to use integer values only but thanks to using ModelClass (irony...) it could be easily changed to floating point values. I've decided to implement change falling/gravity algorithm:
<pre><code class="cpp">
if (isFalling)
{
	if (holdingJumpButton)
	{
		frameMovementUp -= gravitySlow;
		gravityStep = 0;
	}
	else
	{
		gravityStep++;
		frameMovementUp -= gravityNormal * sqrt(min(maxGravityStep, gravityStep));
	}
}
</code></pre>

When player is holding "Jump" button, character is falling slowly. Otherwise character is falling faster until reaches velocity limit. It was jump a simple idea to speed up falling and run away from linear falling which seems unnatural.

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/0.9">https://github.com/komilll/AncientDroneRemake/releases/tag/0.9</a> - Week #9 release (Gameplay ready from now on)
<center><img src="Blog/gif_9.gif"></center>
<center><i>Week #9: Gif #9 – Dark spheres - traps</i></center>

<h2>Week #8</h2>
A grave mistake was basing everything on "ModelClass" which is basically quad that is treated as base graphics unit representing object. Due to overuse, using screen-space [0, 1] became hard and almost impossible or too hard to bother. However it comes with many pros - using only one space and one script to define every model and physics bounds used in game is great thanks to simplicity and reusing many functions.

This week, I've upgraded map parser so enemies can be spawned by simply putting them in Tiled. Creating first map which was a remake of an original one, took me only few minutes and works as intended. I had some difficulties with waypoints for Crows. Therefore I'm using second layer for them - on startup each Crow enemy is looking for nearest waypoint on both left and right side and then uses them through game. Also by testing game in real conditions, many bugs, mistakes were fixed and optimization applied.

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/0.8">https://github.com/komilll/AncientDroneRemake/releases/tag/0.8</a> - Week #8 release (Alpha, .exe application included)
<center><img src="Blog/gif_8.gif"></center>
<center><i>Week #8: Gif #8 – First map recreated and ready to play</i></center>

<h2>Week #7</h2>
Week started with implementing gameplay usage of Health UI Bar and Drone Energy Bar. Refilling Energy is simply streching horizontally model of bar. It's definatelly not the best solution because rendering should just be cut in correct place to avoid stretching texture which looks generally bad. In this case it's not THAT awful but result isn't much satisfying.

Another thing was creating parser for .csv data. I'm going to use Tiled for creating levels for this case. Spawn for player, spawn for enemies and other potential features. Soon after first tests it turned out that physics engine was flawed and I needed to implement much improvements to allow player move smoothly on map generated by Tiled.

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/0.7">https://github.com/komilll/AncientDroneRemake/releases/tag/0.7</a> - Week #7 release
<center><img src="Blog/gif_7.gif"></center>
<center><i>Week #7: Gif #7 – Environment created with Tiled</i></center>

<h2>Week #6</h2>
General class for dynamic objects turned out to be useful because that was pretty good base for building drone and spears it spawns. Drone can be controlled by pressing 'E' key in the place that we want it to be moved. Movement vector (and speed) is based on current distance to destination. It turned out that this solution works better than original one, where movement speed of drone was constant.

Because I didn't created dedicated system for UI, I turned out to be slave od ModelClass. UI is based on sprite placed in a world which creates a problem that changing camera distance will destroy whole UI. A solution for moving camera is taking current camera position and position from previous frame:

<pre><code class="cpp">
if (m_TextureShadersGeneral.at(i)->GetIsConstantOnScreen())
{
	D3DXVECTOR3 posDiff = m_Camera->GetPosition() - m_lastFrameCameraPosition;
	model->SetTranslation(model->GetTranslation().x + posDiff.x, model->GetTranslation().y + posDiff.y, model->GetTranslation().z);
}
</code></pre>

Changing camera distance may take similar aproach. As camera is getting further from player, UI is scaling up and when it's going near player, UI is getting smaller. Of course that system has a lot of flaws because it has to be recalculated every frame (or every frame if there was changes to camera) instead of having constant coordinates. But there is an advantage because everything is based on one class for rendering sprites.

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/0.6">https://github.com/komilll/AncientDroneRemake/releases/tag/0.6</a> - Week #6 release
<center><img src="Blog/gif_6.gif"></center>
<center><i>Week #6: Gif #6 – Using drone controlled by player to kill enemy</i></center>

<h2>Week #5</h2>
Implemented behaviours for all enemies that were created in original game jam version. Although there was also boss, I would rather not implement it because of harder AI, animations and other gameplay difficulties. Purpose of creating this engine is mostly to learn graphics pipeline and creating basic game engine, not to implement full hard gameplay features.

This week was mostly upgrading AI, creating EnemyBase class and parent class for general dynamic objects. Also I've been creating better animation pipeline and implemented all animations for enemies that will be needed until the end of project.

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/0.5">https://github.com/komilll/AncientDroneRemake/releases/tag/0.5</a> - Week #5 release
<center><img src="Blog/gif_5.gif"></center>
<center><i>Week #5: Gif #5 – All enemies animations</i></center>

<h2>Week #4</h2>
And that's finaly time for some gameplay. I've added EnemyWanderer which moves right/left until it meets obstacle. Also it deals damage by dealing player damage on touch. Second enemy is EnemyFlying, that's the crow that flies from waypoint A to B and spawns bombs that explode on impact. Unfortunatelly both of them are not working due to last-minute changes in enemies inheritence changes and pipeline modifications.


There is also an EnemyArcher that is stationary enemy which shoots left/right based on player's relative position. I managed to finish graphics pipeline for it and it works great. However shooting spawned arrow and killing player with it isn't done yet.

Generally I've really enjoyed and I'm really proud of my work this week. It doesn't seem that much changed but I've established universal rendering pipelines for enemies and it's almost done for basic moving objects and that will improve working speed much in the future.

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/0.4">https://github.com/komilll/AncientDroneRemake/releases/tag/0.4</a> - Week #4 release
<center><img src="Blog/gif_4.gif"></center>
<center><i>Week #4: Gif #4 – Enemy attack animation</i></center>

<h2>Week #3</h2>
As I implemented animation pipeline in previous week, I used it for three basic player animations. In case if player is standing still, idle animation is playing. There is also movement animation and jump animation which freezes at last frame and doesn't loop (based on boolean).

Other major improvement was setting transparency on spritesheet. I colored spritesheet transparent color as pink (1, 0, 1) and in pixelshader I'm checking if chosen pixel is transparent or not

<pre><code class="cpp">
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor = shaderTexture.Sample(SampleType, float2(input.tex.x, input.tex.y));	

	float alpha = textureColor.r + textureColor.b;
	textureColor.a = 1.0f - ( alpha - normalize(textureColor.g) * 2.0f );
	textureColor *= normalize(textureColor.a);

   	return textureColor;
}
</code></pre>

First, I am summing red and blue channel (if red = 1.0 and blue = 1.0 then we're getting pink). So we need alpha variable to be 2.0. Next line is really interesing - in the brackets we check if there green channel is 0.0. If green is different, then it will be normalized to 1.0, multiplied to 2.0 and so we will get value equal to 0.0 in brackets and finally alpha channel will be 1.0 so it will be fully opaque. Last line is to convert to black color (0, 0, 0, 0) when alpha is 0.0.

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/0.3">https://github.com/komilll/AncientDroneRemake/releases/tag/0.3</a> - Week #3 release
<center><img src="Blog/gif_3.gif"></center>
<center><i>Week #3: Gif #3 – Player full animations</i></center>

<h2>Week #2</h2>
This week I was preparing animation pipeline. It's based on simply importing whole 2D sprite sheet and slicing it to get frames we'd like to use further. Currently (and it'll be propably enough for this project) there is only "fixed size" slice option, which means that after specifying number of frames, row and size of single sprite, you are getting animation sequence that can be simply used.

It doesn't use any recognizion systems so it has to be controlled by user (but Unity works similar except that it throws away empty frames). Major downside is that there isn't transparency introduced. My idea is to create .dds with pink background and use it as translucent color then, but I'll have to improve animation pipeline first.

I'm kinda disappointed with this week because I had a lot of things going on university and I got a little seek on the weekend so I programmed like 6-7 hours this week but it doesn't mean I didn't like little animation that I've produced and which you can see below.

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/0.2">https://github.com/komilll/AncientDroneRemake/releases/tag/0.2</a> - Week #2 release
<center><img src="Blog/gif_2.gif"></center>
<center><i>Week #2: Gif #2 – Player idle animation</i></center>

<h2>Week #1</h2>
To begin our adventure I’ve prepared basic pipeline for rendering 2D images. Image is created simply by rendering rectangle (2 triangles) with specified size and color. In future I will implement textures (or even .png files) importing to allow using sprites. Base of my pipeline is based on Rasterek tutorial, up until #4 tutorial - <a href="http://www.rastertek.com/dx11tut04.html">http://www.rastertek.com/dx11tut04.html</a>. I highly recommend his tutorial series to begin with DirectX 11 programming.

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

<a href="https://github.com/komilll/AncientDroneRemake/releases/tag/0.1">https://github.com/komilll/AncientDroneRemake/releases/tag/0.1</a> - Week #1 release
<center><img src="Blog/gif_1.gif"></center>
<center><i>Week #1: Gif #1 – Jumping on platforms using basic physics engine</i></center>
