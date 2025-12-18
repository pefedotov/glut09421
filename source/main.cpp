#include <stdio.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <numbers>
#include <string>
#include <random>

constexpr auto win_scale = 2.0;
constexpr auto win_width  = 800;
constexpr auto win_height = 800;
constexpr auto world_width = 300;
constexpr auto world_height = 300;


constexpr auto speed_up    = 10;
constexpr auto speed_down  = 10;
constexpr auto speed_left  = 10;
constexpr auto speed_right = 10;

glm::vec2 view_position{0, 0};

class Shape
{
	glm::vec2 m_position{0,0};
	glm::vec3 m_color;
public:
	Shape() = default;
	Shape(const glm::vec2& pos) : m_position(pos) {}
	Shape(const glm::vec2& pos, const glm::vec3& color) : m_position(pos), m_color(color) {}

	virtual void draw() const = 0;

	auto position() const { return m_position; }
	void set_position(const glm::vec2& pos) { m_position = pos;  }

	auto color() const { return m_color; }
	void set_color(const glm::vec3& color) { m_color = color; }
};


class Triangle : public Shape
{
	glm::vec2  m_p1;
	glm::vec2  m_p2;
	glm::vec2  m_p3;
	
public:
	Triangle(
		const glm::vec2 p1,
		const glm::vec2 p2,
		const glm::vec2 p3,
		const glm::vec3 color = {1, 0, 0}
	) : Shape(p1, color), m_p1(p1), m_p2(p2), m_p3(p3) 
	{
	}

	void draw() const override
	{
		glPushMatrix();
		glColor3f(color().r, color().g, color().b);
		glBegin(GL_TRIANGLES);
			glVertex2f(m_p1.x, m_p1.y);
			glVertex2f(m_p2.x, m_p2.y);
			glVertex2f(m_p3.x, m_p3.y);
		glEnd();
		glPopMatrix();
	}

};

class Circle : public Shape
{
	float m_radius{1};
public:
	Circle(const glm::vec2 position, float radius, glm::vec3 color = {1, 0, 0})
		: Shape(position, color), m_radius(radius) { }


	void draw() const
	{
		glPushMatrix();
		glColor3f(color().r, color().g, color().b);
		glBegin(GL_POLYGON);
			constexpr auto count = 50;
			for (auto i = 0; i < count; ++i)
			{
				const auto theta = 2 * 3.14 * i / count;
				const auto pos = position() + glm::vec2{
				    m_radius * sin(theta),
				    m_radius * cos(theta),
				};
				glVertex2d(pos.x, pos.y);
			}
		glEnd();
		/*glBegin(GL_TRIANGLE_FAN);
		glVertex2f(position().x, position().y);
		for (auto theta = 0; theta <= 361; theta += 1)
		{
			const float x = m_radius * sin(theta * 2 * 3.14 / 360);
			const float y = m_radius * cos(theta * 2 * 3.14 / 360);
			glVertex2f(x, y);
		}
		glEnd();*/
		glPopMatrix();
	}
};


class Button
{
	using vec2 = glm::i64vec2;
	using vec4 = glm::i64vec4;
	vec2        m_left_top{0, 0};
	vec2        m_size    {100, 50};
	// vec4        m_margins { 2, 2, 2, 2 };
	std::string m_text    {};
	bool        m_pressed {false};
	bool        m_visible {true};
public:
	Button(const std::string& text) : m_text(text) {}
	Button(vec2 pos, const std::string& text) : m_left_top(pos), m_text(text) {}
	Button(vec2 pos, vec2 size, const std::string& text)
		: m_left_top(pos), m_size(size), m_text(text) {}

	vec2        left_top () const { return m_left_top; }
	vec2        size	 () const { return m_size	 ; }
	std::string text	 () const { return m_text	 ; }
	bool        pressed	 () const { return m_pressed ; }	
	bool        visible  () const { return m_visible ; }

	void set_left_top (vec2 pos        ) { m_left_top = pos; }
	void set_size     (vec2 size       ) { m_size = size; }
	void set_text     (std::string text) { m_text = text; }
	void set_pressed  (bool pressed    ) { m_pressed = pressed; }
	void set_visible  (bool visible    ) { m_visible  = visible; }



	void draw() const
	{
		if (!m_visible)
			return;

		// if(pressed)
		glPushMatrix();

		glColor3f(0.8f, 0.8f, 0.8f);
		glRectf(
		 	m_left_top.x,
		 	m_left_top.y,
		 	m_left_top.x + m_size.x,
		 	m_left_top.y + m_size.y
		 	);
		glBegin(GL_LINE_LOOP);
			glColor3f(0.9, 0.9, 0.9);
			glVertex2i(
				m_left_top.x, 
				m_left_top.y);
			glColor3f(0.7, 0.7, 0.7);
			glVertex2i(m_left_top.x + m_size.x, m_left_top.y);
			glColor3f(0.7, 0.7, 0.7);
			glVertex2i(m_left_top.x + m_size.x, m_left_top.y + m_size.y);
			glColor3f(0.3, 0.3, 0.3);
			glVertex2i(m_left_top.x, m_left_top.y + m_size.y);
		glEnd();

		glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2i(
				m_left_top.x + m_size.x / 2.,
				m_left_top.y
				);
			glVertex2i(
				m_left_top.x + m_size.x / 2.,
				m_left_top.y + m_size.y
				);
		glEnd();
		glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2i(
				m_left_top.x,
				m_left_top.y + m_size.y / 2.
				);
			glVertex2i(
				m_left_top.x + m_size.x,
				m_left_top.y + m_size.y / 2.
				);
		glEnd();

		if (!m_text.empty())
		{
			const auto font = GLUT_BITMAP_TIMES_ROMAN_24;
			const auto text = (const unsigned char*)m_text.data();
			const auto text_length = glutBitmapLength(font, text);
			const auto text_height = glutBitmapHeight(font);
			glColor3f(1, 0, 0); // text color
			const vec2 text_pos = {
				m_left_top.x + m_size.x / 2. - text_length/4.,
				m_left_top.y + m_size.y / 2. + text_height/8.
			};
			glRasterPos2i(text_pos.x, text_pos.y);
			glutBitmapString(font, text);
		}
		
		glPopMatrix();
	}

	bool test(int x, int y) const
	{
		return (
			m_left_top.x < x && 
			m_left_top.y < y && 
			x < m_left_top.x + m_size.x &&
			y < m_left_top.y + m_size.y
			);
	}

	void click() const
	{
		printf("Clicked!\r\n");
	}

};

// Button btn({ 200, 200 }, {200, 200},  "test test test");
std::vector<Shape*> shapes;


void display()
{
	glClearColor(1.f, 1.f, 1.f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(view_position.x, view_position.y, 0);

	for (auto&& shape : shapes)
	{
		shape->draw();
	}

	//btn.draw();

	glutSwapBuffers();
}


void keyboard(const unsigned char key, const int x, const int y)
{
	printf("key = %c, x = %d, y = %d\r\n", key, x, y);
	switch (key)
	{
	case 'w': 
		view_position.y += speed_up;
		break;
	case 's': 
		view_position.y -= speed_down;
		break;
	case 'a': 
		view_position.x -= speed_left;
		break;
	case 'd': 
		view_position.x += speed_right;
		break;
	} // switch key


	glutPostRedisplay();
}

void mouse_handler(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		// if (btn.test(x, y))
		// 	btn.click();
		break;
	}
}

void timer_create(int)
{	
	const glm::vec2 position = {
		rand() % world_width * 2 - world_width ,
		rand() % world_height * 2 - world_height
	};
	constexpr auto radius = 5;
	auto c = new Circle(position, radius);
	shapes.push_back(c);
	//printf("Circle created (%f,%f)\r\n", c->position().x, c->position().y);
	

	glutPostRedisplay();
	glutTimerFunc(20, timer_create, 0);
}

int main(int argc, char** argv)
{
	srand(time(0));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("GLUTTest");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, win_width/win_scale, win_height/win_scale, 0);
	gluOrtho2D(
		-world_width, +world_width, 
		-world_height, +world_height
		);

	

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse_handler);
	glutTimerFunc(50, timer_create, 0);


	glutMainLoop();

	printf("test\r\n");
	return 0;
}