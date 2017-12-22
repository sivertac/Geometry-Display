//Author: Sivert Andresen Cubedo
#pragma once

#ifndef GeometryDisplay_HEADER
#define GeometryDisplay_HEADER

#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <memory>
#include <cmath>

#include <SFML\Graphics.hpp>

#include <wykobi.hpp>
#include <wykobi_algorithm.hpp>

namespace GeometryDisplay {
	
	class DrawObject {
	public:
		std::string name;
		sf::Color color = sf::Color::White;
		bool outer_line = false;
		virtual void appendVertex(sf::VertexArray & vertex_arr) = 0;
		virtual DrawObject* clone() = 0;
	};
	class TriangleShape : public DrawObject {
	public:
		wykobi::triangle<float, 2> triangle;
		TriangleShape(float x0, float y0, float x1, float y1, float x2, float y2);
		TriangleShape(std::vector<sf::Vector2f> & vec);
	};
	class PolygonShape : public DrawObject {
	public:
		wykobi::polygon<float, 2> polygon;
		PolygonShape(wykobi::polygon<float, 2> poly);
		PolygonShape* clone() override;
		void appendVertex(sf::VertexArray & vertex_arr) override;
	};
	class LineShape : public DrawObject {
	public:
		wykobi::segment<float, 2> segment;
		float thickness = 1.f;
		LineShape(wykobi::segment<float, 2> seg);
		LineShape* clone() override;
		void appendVertex(sf::VertexArray & vertex_arr) override;
	};

	class Window {
	private:
		sf::RenderWindow window;

		std::string window_title = "Geometry Display FeelsGoodMan Clap";

		int update_interval = 50;				//in ms
		unsigned int window_width = 500;		//in px
		unsigned int window_height = 500;		//in px
		bool update_settings = false;
		bool update_frame = false;
		bool running = true;

		std::mutex window_mutex;

		std::mutex draw_object_vec_mutex;
		std::vector<std::unique_ptr<DrawObject>> draw_object_vec;
		sf::VertexArray draw_object_vertex_array = sf::VertexArray(sf::Triangles);
		//std::vector<sf::Text> shape_text_vec;
		
		sf::VertexArray ui_vertex_array = sf::VertexArray(sf::Triangles);
		std::vector<sf::Text> ui_text_vector;
		float ui_border_thickness = 50.f;
		sf::Color ui_border_color = sf::Color(129, 129, 129);

		std::thread window_thread;

		sf::VertexArray diagram_vertex_array = sf::VertexArray(sf::Triangles);

		//relation between screen space and object space
		wykobi::point2d<float> diagram_position = wykobi::make_point<float>(0.f, 0.f);	//diagram world position, at origin
		wykobi::polygon<float, 2> diagram_world_area;	//world area
		wykobi::vector2d<float> diagram_world_zoom = wykobi::make_vector<float>(1.f, 1.f);		//world zoom
		wykobi::vector2d<float> diagram_world_size;
		float diagram_world_rotation = 0.f;
		wykobi::rectangle<float> diagram_screen_area;	//area on screen
		float diagram_screen_rotation = 0.f;			
		std::size_t diagram_screen_origin_corner = 0;
		
		wykobi::vector2d<float> diagram_line_resolution = wykobi::make_vector<float>(10.f, 10.f);
		float diagram_line_thickness = 1.f;
		sf::Color diagram_line_color = sf::Color::Blue;

		

		/*
		Window thread function
		*/
		void windowHandler();

		/*
		Update diagram
		*/
		void updateDiagram();

		/*
		Render diagram
		*/
		void renderDiagram();

		/*
		Render UI
		*/
		void renderUI();

		/*
		Renders and displays next frame
		Must be called from window_thread
		*/
		void renderDrawObject();
		
	public:
		Window();							//constructor

		void addShape(DrawObject & shape);
		void addShape(wykobi::polygon<float, 2> poly);
		void addShape(wykobi::segment<float, 2> seg);
		//std::vector<Shape*> addShape(std::vector<wykobi::polygon<float, 2>> & vec);
		void clearShapeVec();

		/*
		Set diagram render corner
		*/
		void setDiagramOriginCorner(std::size_t i);

		/*
		Set diagram rotation
		*/
		void setDiagramRotaton(float r);

		/*
		Set diagram resolution
		*/
		void setDiagramLineResolution(float x, float y);

		/*
		Set diagram position
		*/
		void setDiagramPosition(float x, float y);

		/*
		Set update interval
		In milliseconds
		*/
		void setUpdateInterval(int t);

		/*
		Set window size
		*/
		void setSize(int w, int h);

		/*
		Set title
		*/
		void setTitle(std::string title);

		/*
		Get window width
		*/
		int getWindowWidth();

		/*
		Get window height
		*/
		int getWindowHeight();

		/*
		Wait for window_thread to close
		This will block if the thread is joinable, else nothing happens
		*/
		void join();

		/*
		Init display
		*/
		void create();										

		/*
		Close display
		Kill thread
		*/
		void close();						
	};


	/*
	Make two triangles representing a line with thickness
	*/
	std::vector<wykobi::triangle<float, 2>> makeTriangleLine(float x0, float y0, float x1, float y1, float thickness);
	std::vector<wykobi::triangle<float, 2>> makeTriangleLine(wykobi::segment<float, 2> & seg, float thickness);

}

#endif // !GeometryDisplay_HEADER


//end
