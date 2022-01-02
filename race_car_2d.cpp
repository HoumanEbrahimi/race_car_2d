// race_car_2d.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Header.h"
#include <vector>
#include <time.h>
#include <string>
using namespace std;
class race_car : public olcConsoleGameEngine {
public:
    race_car() {
        m_sAppName = L"Formula";
    }
private:
    float car_pos = 0.0f;
    float distance = 0.0f;
    float curvature = 0.0f;

    float track_curvature = 0.0f;
    float player_curvature = 0.0f;

    float car_speed=0.0f;

    int car_state = 0;
    // coordinates for the obstacle 

    float obs_pos = 0.0f;

    float height_obstacle = 50.0f;

    vector <pair<float, float>> track;

    

protected:
    virtual bool OnUserCreate() override {

        track.push_back(make_pair(0.0f, 10.0f));		// Short section for start/finish line
        track.push_back(make_pair(0.0f, 200.0f));
        track.push_back(make_pair(1.0f, 200.0f));
        track.push_back(make_pair(0.0f, 400.0f));
        track.push_back(make_pair(-1.0f, 100.0f));
        track.push_back(make_pair(0.0f, 200.0f));
        track.push_back(make_pair(-1.0f, 200.0f));
        track.push_back(make_pair(1.0f, 200.0f));
        track.push_back(make_pair(0.0f, 200.0f));
        track.push_back(make_pair(0.2f, 500.0f));
        track.push_back(make_pair(0.0f, 200.0f));



        return true;

    }

    virtual bool OnUserUpdate(float elapsed_time) override {
        Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);
        height_obstacle += 1.5f * elapsed_time * 8.5f;

        float obstacle_pos = ScreenWidth() / 2 + ((int)(ScreenWidth() * obs_pos));
        ;

       if (height_obstacle >= ScreenHeight()) {
           height_obstacle = 50;

       }



        if (m_keys[VK_UP].bHeld) {
            car_speed += 2.0f * elapsed_time;
            car_state = 0;
        }
        else {
            car_speed = 0.4f;
            car_state = 0;

        }

            
        distance += (7.0f*car_speed) * elapsed_time;

        if (m_keys[VK_RIGHT].bHeld) {
            car_state = 1;
            player_curvature += 0.7f*elapsed_time ;
        }

        if (m_keys[VK_LEFT].bHeld) {
            car_state = 2;
            player_curvature -= 0.7f*elapsed_time ;
        }


        int track_section = 0;
        float offset = 0;

        while (track_section<track.size() && offset < distance) {
            offset += track[track_section].second;
            track_section++;
        }
        float target_curve = track[track_section-1].first;
        float target_difference = (target_curve - curvature) * elapsed_time;
        curvature += target_difference;
        track_curvature += (curvature)*elapsed_time;

        for (int y = 0; y < ScreenHeight() / 2; y++) {
            int sky = ScreenHeight() / 2 - y;
            for (int x = 0; x < ScreenWidth(); x++) {
                Draw(x, sky, PIXEL_SOLID, FG_DARK_BLUE);
            }
        }
        for (int a = 0; a < ScreenWidth(); a++) {
            int hill = (int)(fabs(sinf(a * 0.01f + track_curvature) * 16.0f));

            for (int b = (ScreenHeight() / 2) - hill; b < ScreenHeight() / 2 + 3; b++) {
                Draw(a, b, PIXEL_SOLID, FG_DARK_YELLOW);
            }


        }

        
        obstacle_pos *= 2;
        int nCarPos = ScreenWidth() / 2 + ((int)(ScreenWidth() * car_pos) / 2.0) - 7;
        for (int y = 0; y < ScreenHeight()/2; y++) {
            for (int x = 0; x < ScreenWidth(); x++) {

                


                

                float perspective = float(y) / (ScreenHeight() / 2.0f);
                float mid_point = 0.5f+curvature*powf((1.0f-perspective),3);
           



               
                float road_width =(0.2f+perspective)*0.8f;
                float clip_width = road_width * 0.15f;
                road_width *= 0.5f;



                int left_grass = (mid_point- road_width - clip_width) * ScreenWidth();
                int left_clip = (mid_point - road_width )*ScreenWidth();
                int right_grass = (mid_point + road_width  + clip_width) * ScreenWidth();
                int right_clip = (mid_point + road_width ) * ScreenWidth();



                int change_grass_color= 8 * sinf((0.5f + perspective) * distance) + 1>0.0f? FG_GREEN: FG_DARK_GREEN;
                int change_clip_color = sinf(80.0f * powf(1.0f - perspective, 2) + distance) > 0.0f ? FG_RED : FG_WHITE;
                int midway_from_x = ScreenHeight() / 2 + y;


                if (x >= 0 && x <= left_grass) {
                    Draw(x,midway_from_x , PIXEL_SOLID, change_grass_color);
                }

                if (x > left_grass && x <= left_clip) {
                    Draw(x, midway_from_x, PIXEL_SOLID, change_clip_color);
                }
 
                if (x > left_clip && x < right_clip) {
                    Draw(x, midway_from_x, PIXEL_SOLID, FG_DARK_GREY);

                }

                if (x >= right_clip && x < right_grass) {
                    Draw(x, midway_from_x, PIXEL_SOLID, change_clip_color);
                }

                if (x >= right_grass && x < ScreenWidth()) {
                    Draw(x, midway_from_x, PIXEL_SOLID, change_grass_color);

                } 
            }

            car_pos = player_curvature - (0.7f*track_curvature);
            



            switch (car_state) {
            case 0:
                DrawStringAlpha(nCarPos, 80, L"   ||####||   ");
                DrawStringAlpha(nCarPos, 81, L"      ##      ");
                DrawStringAlpha(nCarPos, 82, L"     ####     ");
                DrawStringAlpha(nCarPos, 83, L"     ####     ");
                DrawStringAlpha(nCarPos, 84, L"|||  ####  |||");
                DrawStringAlpha(nCarPos, 85, L"|||########|||");
                DrawStringAlpha(nCarPos, 86, L"|||  ####  |||");
                break;
            case 1:
                DrawStringAlpha(nCarPos, 80, L"      //####//");
                DrawStringAlpha(nCarPos, 81, L"         ##   ");
                DrawStringAlpha(nCarPos, 82, L"       ####   ");
                DrawStringAlpha(nCarPos, 83, L"      ####    ");
                DrawStringAlpha(nCarPos, 84, L"///  ####//// ");
                DrawStringAlpha(nCarPos, 85, L"//#######///O ");
                DrawStringAlpha(nCarPos, 86, L"/// #### //// ");
                break;

            case 2:
                DrawStringAlpha(nCarPos, 80, L"\\\\####\\\\      ");
                DrawStringAlpha(nCarPos, 81, L"   ##         ");
                DrawStringAlpha(nCarPos, 82, L"   ####       ");
                DrawStringAlpha(nCarPos, 83, L"    ####      ");
                DrawStringAlpha(nCarPos, 84, L" \\\\\\\\####  \\\\\\");
                DrawStringAlpha(nCarPos, 85, L" O\\\\\\#######\\\\");
                DrawStringAlpha(nCarPos, 86, L" \\\\\\\\ #### \\\\\\");
                break;



            }

            DrawString(0, 0,to_wstring(obstacle_pos));

              
            DrawStringAlpha(obstacle_pos, height_obstacle, L"    ######   ");
            DrawStringAlpha(obstacle_pos, height_obstacle+1, L"    ######      ");
            DrawStringAlpha(obstacle_pos, height_obstacle+2, L"   # #### #     ");
            DrawStringAlpha(obstacle_pos, height_obstacle+3, L"   ########     ");
            DrawStringAlpha(obstacle_pos, height_obstacle+4, L"    ##  ##     ");

        }
        return true;
    }
};
int main()
{
    srand(time(NULL));
    race_car game;
    game.ConstructConsole(160, 100, 8, 8);
    game.Start();
   
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
