#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include<bits/stdc++.h>

#include"bitmap_image.hpp"


using namespace std;

#include "1905116_matrix.cpp"

// #define M_PI 2.0*acos(0.0)

struct Fragment {
    int x, y;
    double z;

    bool operator<(const Fragment &other) const {
        return z > other.z;
    }
};

/*
a = a.x
b = b.x
c = a.y
d = b.y
*/
double interpolation(double a, double b, double c, double d,double cor){
    double val = a + (b - a) * (cor - c)/ (d - c);
    return val;
}

int main(){
    ifstream fin("scene.txt");
    ofstream fout("stage1.txt");
    
    Point eye, look, up;
    double fovY, aspectRatio, near, far;
    fin>>eye>>look>>up;
    fin>>fovY>>aspectRatio>>near>>far;

    stack<Matrix> s;
    Matrix I;
    I.identity();
    s.push(I);

    string command;
    while(fin>>command){
        if( command == "triangle" ){
            Triangle t;
            fin>>t;
            cout<<"input:"<<endl;
            cout<<t<<endl;
            cout<<"s:"<<endl;
            cout<<s.top()<<endl;
            
            t = s.top() * t;
            cout<<t<<endl;
            fout<<t;
            fout<<endl;
        } else if( command == "translate" ){
            Point p;
            fin>>p;
            Matrix m;
            m.translate(p);
            s.top() = s.top()*m;
        } else if( command == "scale" ){
            Point p;
            fin>>p;
            Matrix m;
            m.scale(p);
            s.top() = s.top()*m;
        } else if( command == "rotate" ){
            Point p;
            double theta;
            fin>>theta>>p;
            theta = theta*M_PI/180.0;
            Matrix m;
            m.rotate(p, theta);
            s.top() = s.top()*m;
        } else if( command == "push" ){
            s.push(s.top());
        } else if( command == "pop" ){
            s.pop();
        } else if( command == "end" ){
            break;
        }
    }
    fin.close();
    fout.close();

    fin.open("stage1.txt");
    fout.open("stage2.txt");

    Point l, r, u;

    l = look - eye;
    l.normalize();
    r = l ^ up;
    r.normalize();
    u = r ^ l;
    u.normalize();

    Point t_eye;
    t_eye = eye * -1;
    
    Matrix T;
    T.translate(t_eye);

    Matrix R;
    R.tranformation(l, r, u);
    
    Matrix V = R * T;

    Triangle triangle;

    while(fin>> triangle){
        triangle = V * triangle;
        fout<< triangle<< endl;
    }

    fin.close();
    fout.close();


    fin.open("stage2.txt");
    fout.open("stage3.txt");

    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY * M_PI / 360);
    double r_t = near * tan(fovX * M_PI / 360);

    Matrix P;
    P.matrix[0][0] = near / r_t;
    P.matrix[1][1] = near / t;
    P.matrix[2][2] = - (far + near) / (far - near);
    P.matrix[2][3] = - (2 * far * near) / (far - near);
    P.matrix[3][2] = -1;


    while(fin>>triangle){
        triangle = P*triangle;
        fout<<triangle<<endl;
    }

    fin.close();
    fout.close();

    fin.open("stage3.txt");
    fout.open("z_buffer.txt");
    ifstream fin2("config.txt");
    int s_w, s_h;
    fin2>>s_w>>s_h;

    double left_limit = -s_w/s_w;
    double right_limit = s_w/s_w;
    double bottom_limit = -s_h/s_h;
    double top_limit = s_h/s_h;
    double z_min=-1, z_max=1;
    double dx = (right_limit-left_limit)/s_w;
    double dy = (top_limit-bottom_limit)/s_h;
    double top_Y = top_limit - dy/2.0;
    double left_X = left_limit + dx/2.0;


    // initialize z-buffer and frame buffer

    vector<double> sc_z(s_w, z_max);

    vector< vector<double> > z_buffer(s_h, sc_z);
    for(int i=0; i<s_h; i++){
        for(int j=0; j<s_w; j++){
            z_buffer[i][j] = z_max;
        }
    }

    bitmap_image image(s_w, s_h);
    image.set_all_channels(0.0, 0.0, 0.0);


    while(fin>>triangle){

        triangle.sort();
        triangle.recolor();
        // clipping
        double miny, maxy;

        // scan conversion

        std::priority_queue<Fragment> fragments;

        

        miny = triangle.b.y;
        maxy = triangle.c.y;

       
        miny = max(miny, bottom_limit);
        maxy = min(maxy, top_limit);


        // scan conversion
        for(double y=miny; y<=maxy; y+=dy){
           
            double minx = 0;
            double maxx = -1;
            double minz = 0;
            double maxz = -1;


            if( triangle.b.y != triangle.c.y and triangle.a.y != triangle.c.y ){
                minx = interpolation(triangle.c.x, triangle.b.x, triangle.c.y, triangle.b.y, y);
                maxx = interpolation(triangle.a.x, triangle.c.x, triangle.a.y, triangle.c.y, y);

                minz = interpolation(triangle.c.z, triangle.b.z, triangle.c.y, triangle.b.y, y);
                maxz = interpolation(triangle.a.z, triangle.c.z, triangle.a.y, triangle.c.y, y);

                if( minx > maxx ) {
                    swap(minx, maxx);
                    swap(minz, maxz);
                }
            }
            minx = max(minx, left_limit);
            maxx = min(maxx, right_limit);

            for(double x=minx; x<=maxx; x+=dx){
                if( maxx == minx ) continue;
                
                int i = (top_Y-y)/dy;
                int j = (x-left_X)/dx;
                
                double z = minz + (maxz-minz)*(x-minx)/(maxx-minx);
                if( z < z_buffer[i][j] and z>z_min ){
                    z_buffer[i][j] = z;
                    fragments.push({j, i ,z});
                }
                
            }
        }
        while (!fragments.empty()) {
            int j = fragments.top().x;
            int i = fragments.top().y;
            fragments.pop();

            image.set_pixel(j, i, triangle.color.r, triangle.color.g, triangle.color.b);
        }


    }

    image.save_image("out.bmp");

    for(int i=0; i<s_h; i++){
        for(int j=0; j<s_w; j++){
            if (z_buffer[i][j] < z_max) {
                fout << setprecision(6) << fixed << z_buffer[i][j] << "\t";
            }
        }
        fout<<endl;
    }


    fin.close();
    fin2.close();
    fout.close();
    
    z_buffer.clear();
    z_buffer.shrink_to_fit();
    
    return 0;

}