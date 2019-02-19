	CSCI3260 Assignment 1 Keyboard / Mouse Events  

Name: LEE Kwun Po
Student ID:1155093052

Manipulation:
// for key a, d, w, s moving cubes and rotate everything
	if (key == 'a')
	{
		r += 0.1f;
		dx -= 0.03f;
		dyevil -= 0.03f; 

	}
	if (key == 'd')
	{
		r -= 0.1f;
		dx += 0.03f;
		dyevil += 0.03f;
	}
	if (key == 'w')
	{
		r += 0.1f;
		dy += 0.03f;
		dxevil -= 0.03f;

	}
	if (key == 's')
	{
		r -= 0.1f;
		dy -= 0.03f;
		dxevil += 0.03f;
	}
	if (key == 'r')
	{
		r, dx, dy, dxevil, dyevil, k = 0.0f; // reset
	}
for keys all below except f , changing the projection matrix of the object
	if (key == 'o')
	{
		fov += 1.0f;
	}
	if (key == 'p')
	{
		fov -= 1.0f;
	}
	if (key == 'i')
	{
		q += 1.0f;
	}
	if (key == 'k')
	{
		q -= 1.0f;
	}
	if (key == 'l')
	{
		w += 1.0f;
	}
	if (key == ';')
	{
		w -= 1.0f;
	}
	if (key == '[')
	{
		e += 1.0f;
	}
	if (key == ']')
	{
		e -= 1.0f;
	}
	if (key == 'f')
	{
		x += 0.1f;
	}
some functions still not working and I dont know why. Please take a kindly look for my code and give feedbacks. Thank you so much.