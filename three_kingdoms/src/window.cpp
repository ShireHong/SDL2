#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "window.h"
#include "rect.h"
using namespace std;

unique_ptr<SDL_Window, void (*)(SDL_Window*)> Window::mWindow 
        = unique_ptr<SDL_Window, void (*)(SDL_Window*)>(nullptr,SDL_DestroyWindow);
unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> Window::mRenderer 
        = unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)>(nullptr,SDL_DestroyRenderer);
Timer Window::mTimer;
int Window::mFrame;
//other static members
Recti Window::mBox;
int Window::ScrWidth;
int Window::ScrHeight;

SDL_Texture* Window::SDL_tex_loader(const char *path)
{
	return IMG_LoadTexture(mRenderer.get(), path);
}

void Window::set_color(int color) 
{
	tmx_col_bytes col = tmx_col_to_bytes(color);
	SDL_SetRenderDrawColor(mRenderer.get(), col.r, col.g, col.b, col.a);
}

void Window::draw_polyline(double **points, double x, double y, int pointsc) 
{
	int i;
	for (i=1; i<pointsc; i++)
	{
		SDL_RenderDrawLine(mRenderer.get(), x+points[i-1][0], y+points[i-1][1], x+points[i][0], y+points[i][1]);
	}
}

void Window::draw_polygon(double **points, double x, double y, int pointsc) 
{
	draw_polyline(points, x, y, pointsc);
	if (pointsc > 2) 
	{
		SDL_RenderDrawLine(mRenderer.get(), x+points[0][0], y+points[0][1], x+points[pointsc-1][0], y+points[pointsc-1][1]);
	}
}

void Window::draw_objects(tmx_object_group *objgr) 
{
	SDL_Rect rect;
	set_color(objgr->color);
	tmx_object *head = objgr->head;
	while (head)
	{
		if (head->visible) {
			if (head->obj_type == OT_SQUARE) 
			{
				rect.x =     head->x;  rect.y =      head->y;
				rect.w = head->width;  rect.h = head->height;
				SDL_RenderDrawRect(mRenderer.get(), &rect);
			}
			else if (head->obj_type  == OT_POLYGON) 
			{
				draw_polygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
			}
			else if (head->obj_type == OT_POLYLINE) 
			{
				draw_polyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len);
			}
			else if (head->obj_type == OT_ELLIPSE) 
			{
				/* FIXME: no function in SDL2 */
			}
		}
		head = head->next;
	}
}

void Window::draw_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags) 
{
	SDL_Rect src_rect, dest_rect;
	src_rect.x = sx;
	src_rect.y = sy;
	src_rect.w = dest_rect.w = sw;
	src_rect.h = dest_rect.h = sh;
	dest_rect.x = dx;
	dest_rect.y = dy;
	SDL_RenderCopy(mRenderer.get(), (SDL_Texture*)image, &src_rect, &dest_rect);
}

void Window::draw_layer(tmx_map *map, tmx_layer *layer) 
{
	unsigned long i, j;
	unsigned int gid, x, y, w, h, flags;
	float op;
	tmx_tileset *ts;
	tmx_image *im;
	void* image;
	op = layer->opacity;
	for (i=0; i<map->height; i++) 
	{
		for (j=0; j<map->width; j++) 
		{
			gid = (layer->content.gids[(i*map->width)+j]) & TMX_FLIP_BITS_REMOVAL;
			if (map->tiles[gid] != NULL) 
			{
				ts = map->tiles[gid]->tileset;
				im = map->tiles[gid]->image;
				x  = map->tiles[gid]->ul_x;
				y  = map->tiles[gid]->ul_y;
				w  = ts->tile_width;
				h  = ts->tile_height;
				if (im) {
					image = im->resource_image;
				}
				else {
					image = ts->image->resource_image;
				}
				flags = (layer->content.gids[(i*map->width)+j]) & ~TMX_FLIP_BITS_REMOVAL;
				draw_tile(image, x, y, w, h, j*ts->tile_width, i*ts->tile_height, op, flags);
			}
		}
	}
}

void Window::draw_image_layer(tmx_image *image) 
{
	SDL_Rect dim;
	dim.x = dim.y = 0;

	SDL_Texture *texture = (SDL_Texture*)image->resource_image; // Texture loaded by libTMX
	SDL_QueryTexture(texture, NULL, NULL, &(dim.w), &(dim.h));
	SDL_RenderCopy(mRenderer.get(), texture, NULL, &dim);
}

void Window::draw_all_layers(tmx_map *map, tmx_layer *layers) 
{
	while (layers) 
	{
		if (layers->visible) 
		{

			if (layers->type == L_GROUP) 
			{
				draw_all_layers(map, layers->content.group_head);
			}
			else if (layers->type == L_OBJGR)
			{
				draw_objects(layers->content.objgr);
			}
			else if (layers->type == L_IMAGE)
			{
				draw_image_layer(layers->content.image);
			}
			else if (layers->type == L_LAYER)
			{
				draw_layer(map, layers);
			}
		}
		layers = layers->next;
	}
}


void Window::Init(string title, int w, int h)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
        throw runtime_error("SDL Init Failed");
	}
    if(TTF_Init() == -1)
	{
        throw runtime_error("TTF Init Failed");
	}

    ScrWidth = w;
    ScrHeight = h;
    mWindow.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        ScrWidth, ScrHeight, SDL_WINDOW_SHOWN));
    if(mWindow == nullptr)
	{
        throw runtime_error("Fail to open window");
	}

    mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (mRenderer == nullptr)
	{
		throw std::runtime_error("Failed to start renderer");
	}

	mBox.Set(0, 0, ScrWidth, ScrHeight);

	mTimer.Start();
}

void Window::Quit()
{
    //Release the window and renderer
    mRenderer.~unique_ptr();
    mWindow.~unique_ptr();
    TTF_Quit();
    SDL_Quit();
}

void Window::renderTexture(SDL_Texture *tex, int x, int y, SDL_Rect *clip)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != NULL){
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    SDL_RenderCopy(mRenderer.get(), tex, clip, &dst);
}


void Window::DrawTexture(SDL_Texture *tex, const Rectf &dstRect, Recti *clip,
                    float angle, Vector2f pivot, SDL_RendererFlip flip)
{
    //Calculate the pivot point as an offset from image center

	SDL_Point p = {pivot.x, pivot.y};
	p.x += dstRect.w / 2;
	p.y += dstRect.h / 2;
#if 0
    pivot.x += dstRect.w / 2;
    pivot.y += dstRect.h / 2;
#endif
    //Make sure we don't try to convert a NULL clip rect
    SDL_Rect tempClip;
    if (clip != NULL)
	{
    	tempClip = (SDL_Rect)*clip;
	}

	SDL_Rect dst;
	dst.x = dstRect.pos.x;
	dst.y = dstRect.pos.y;
	dst.w = dstRect.w;
	dst.h = dstRect.h;

    //Draw the texture
    SDL_RenderCopyEx(mRenderer.get(), tex, &tempClip, &dst, angle, &p/*&(SDL_Point)pivot*/, flip);
}

void Window::DrawTexture_ex(SDL_Texture *tex, SDL_Rect *dstRect, SDL_Rect *clip,
                    float angle, const SDL_Point *center, SDL_RendererFlip flip)
{
	SDL_Point p ;
	p.x += clip->w / 2;
	p.y += clip->h / 2;
    SDL_RenderCopyEx(mRenderer.get(), tex, dstRect, clip, angle, &p, flip);
}

SDL_Texture* Window::LoadTexture(std::string file){
    SDL_Texture *tex = nullptr;
    tex = IMG_LoadTexture(mRenderer.get(), file.c_str());
    return tex;
}
SDL_Texture* Window::RenderText(std::string message, std::string fontFile, 
                                SDL_Color color, int fontSize)
{
    //Open the font
    TTF_Font *font = nullptr;
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    
    //Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer.get(), surf);
    //Clean up unneeded stuff
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);

    return texture;
}
SDL_Texture* Window::SurfaceToTexture(SDL_Surface *surf){
    SDL_Texture *tex = nullptr;
    tex = SDL_CreateTextureFromSurface(mRenderer.get(), surf);
    SDL_FreeSurface(surf);
    return tex;
}

Recti Window::Box()
{
    //Update the box to match the current window w/h
    SDL_GetWindowSize(mWindow.get(), &mBox.w, &mBox.h);
    return mBox;
}

void Window::ShowAvgFps(bool log)
{
    ++mFrame;
    if (mTimer.Ticks() > 5000){
        float fps = mFrame / (mTimer.Ticks() / 5000.0f);
        std::stringstream ss;
        ss << "Average FPS: " << fps;
        if (log)
		{
            //Debug::Log(ss.str());
			std::cout<< " " << std::endl;
		}
        else
		{
            std::cout<< ss.str() << std::endl;
		}

        mTimer.Start();
        mFrame = 0;
    }
}


void Window::Clear(){
    SDL_RenderClear(mRenderer.get());
}
void Window::Present(){
    SDL_RenderPresent(mRenderer.get());
}
