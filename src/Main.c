#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include <string.h>
#include <unistd.h>


typedef struct Donut {
    Vec3 p;
    float r;
    float br;
} Donut;

Vec3 Sun = {1.0f,0.0f,0.0f};
Donut d = {{0.0f,0.0f,0.0f},0.2f,0.1f};
float ThetaX = 0.0f;
float ThetaY = 0.0f;

void RenderPart(Vec3 p,Pixel c){
    p = Vec3_Add(p,(Vec3){0.5f,0.5f,0.0f});
    p = Vec3_Mul(p,(Vec3){GetWidth(),GetHeight(),1.0f});
    RenderPixel(p.x,p.y,c);
}
void RenderLinePart(Vec3 o,Vec3 p1,Vec3 p2,Pixel c){
    p1 = Vec3_Add(p1,(Vec3){0.5f,0.5f,0.0f});
    p1 = Vec3_Mul(p1,(Vec3){GetWidth(),GetHeight(),1.0f});
    p2 = Vec3_Add(p2,(Vec3){0.5f,0.5f,0.0f});
    p2 = Vec3_Mul(p2,(Vec3){GetWidth(),GetHeight(),1.0f});

    //float l = Vec3_Dot(Vec3_Norm(Sun),Vec3_Norm(Vec3_Perp(Vec3_Sub(p2,p1))));
    float l = (Vec3_Norm(Vec3_Sub(o,p1)).x + 1.0f);
    l = F32_Max(F32_Min(l,1.0f),0.0f);
    Color col = { .p = c };
    col.r = (unsigned char)((float)col.r * l);
    col.g = (unsigned char)((float)col.g * l);
    col.b = (unsigned char)((float)col.b * l);
    RenderLine(((Vec2){p1.x,p1.y}),((Vec2){p2.x,p2.y}),col.p,2.0f);
}

Vec3 Calc(float i,float ax,float ay,float az){
    M3x3 rotX = M3x3_RotateX(ax);
    M3x3 rotY = M3x3_RotateY(ay);
    M3x3 rotZ = M3x3_RotateZ(az);
    Vec3 e = Vec3_Mulf((Vec3){ cosf(i),sinf(i),0.0f },d.br);
    e = M3x3_VecMul(e,rotY);
    return e;
}

void RenderDonut(Donut d,float ThetaX,float ThetaY,float Steps){
    for(float i = 0.0f;i<2.0f * F32_PI;i+=Steps){
        M3x3 rotY = M3x3_RotateY(ThetaY);
        M3x3 rotX = M3x3_RotateX(ThetaX);
        M3x3 rotZ = M3x3_RotateZ(ThetaY);
        
        Vec3 p = Vec3_Mulf((Vec3){cosf(i),0.0f,sinf(i)},d.r);

        Vec3 First = Vec3_Add(p,Calc(0.0f,0.0f,i,0.0f));
        First = M3x3_VecMul(First,rotX);
        First = M3x3_VecMul(First,rotZ);
        Vec3 Before = First;
        
		for(float j = 0.1f;j<=2.0f * F32_PI;j+=0.2f){
            Vec3 e = Vec3_Add(p,Calc(j,0.0f,i,0.0f));
            e = M3x3_VecMul(e,rotX);
            e = M3x3_VecMul(e,rotZ);
            
            RenderLinePart(d.p,Vec3_Add(d.p,Before),Vec3_Add(d.p,e),BROWN);
            Before = e;
        }
        RenderLinePart(d.p,Vec3_Add(d.p,First),Vec3_Add(d.p,Before),BROWN);
    }
}

void Setup(){
    
}
void Update(AlxWindow* w){
	if(Stroke('W').DOWN){
        ThetaX -= 1.0f * w->ElapsedTime;
        //d.p.y -= 1.0 * w->ElapsedTime;
    }
    if(Stroke('S').DOWN){
        ThetaX += 1.0f * w->ElapsedTime;
        //d.p.y += 1.0 * w->ElapsedTime;
    }
    if(Stroke('A').DOWN){
        ThetaY -= 1.0f * w->ElapsedTime;
        //d.p.x -= 1.0 * w->ElapsedTime;
    }
    if(Stroke('D').DOWN){
        ThetaY += 1.0f * w->ElapsedTime;
        //d.p.x += 1.0 * w->ElapsedTime;
    }

    ThetaX += 1.0f * w->ElapsedTime;
    ThetaY -= 0.5f * w->ElapsedTime;

    Clear(BLUE);

    RenderDonut(d,ThetaX,ThetaY,0.01f);
}
void Delete(AlxWindow* w){
	
}

int main(){
    if(Create("Donut",1200,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}