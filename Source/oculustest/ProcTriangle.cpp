// Fill out your copyright notice in the Description page of Project Settings.

#include "oculustest.h"
#include "ProcTriangle.h"

class proc_mesh {
public: 
	TArray<FVector> vertices;
	TArray<int32> triangles;

	proc_mesh operator+(proc_mesh o) {
		proc_mesh temp;
		temp.vertices.Append(this->vertices);
		temp.vertices.Append(o.vertices);
		temp.triangles.Append(this->triangles);
		temp.triangles.Append(o.triangles);
		return temp;
	}
};

TArray<int32> triangle_helper(int offset) {
	TArray<int32> triangles;
	int start = offset * 4; 
	triangles.Add(0 + start);
	triangles.Add(1 + start);
	triangles.Add(2 + start);
	triangles.Add(1 + start);
	triangles.Add(2 + start);
	triangles.Add(3 + start);
	triangles.Add(2 + start);
	triangles.Add(3 + start);
	triangles.Add(0 + start);
	triangles.Add(3 + start);
	triangles.Add(0 + start);
	triangles.Add(1 + start);
	triangles.Add(1 + start);
	triangles.Add(0 + start);
	triangles.Add(3 + start);
	triangles.Add(0 + start);
	triangles.Add(3 + start);
	triangles.Add(2 + start);
	triangles.Add(3 + start);
	triangles.Add(2 + start);
	triangles.Add(1 + start);
	triangles.Add(2 + start);
	triangles.Add(1 + start);
	triangles.Add(0 + start);
	return triangles;
}

TArray<int32> all_triangles(int n) {
	TArray<int32> triangles = triangle_helper(0);
	for (int i = 1; i < n; i++) {
		triangles.Append(triangle_helper(i));
	}
	return triangles;
}

TArray<FVector> line_helper(FVector a, FVector b, FVector c, FVector d) {
	TArray<FVector> vertices;
	vertices.Add(a);
	vertices.Add(b);
	vertices.Add(c);
	vertices.Add(d);

	return vertices;
}

TArray<FVector> line(FVector a, FVector b, float width) {
	return line_helper(a - FVector(width, 0, width), a + FVector(width, 0, width), b + FVector(width, 0, width), b - FVector(width, 0, width));
}

TArray<FVector> triangle(FVector a, FVector b, FVector c) {
	TArray<FVector> vec = line(a, b, 5);
	vec.Append(line(b, c, 5));
	vec.Append(line(a, c, 5));
	return vec;
}

FVector far_corner(int32 upside_down, float length, FVector corner) {
	return FVector(corner.X, corner.Y + length, corner.Z);
}
FVector high_corner(int32 upside_down, float length, FVector corner) {
	float base_to_height = sqrtf((float)3) / 2.0;
	return FVector(corner.X, corner.Y + length / 2, corner.Z+base_to_height*length*upside_down);
}

TArray<FVector> equilateral_triangle(int32 upside_down, float length, FVector corner) {
	float base_to_height = sqrtf((float)3) / 2.0;
	return triangle(corner, far_corner(upside_down, length, corner), high_corner(upside_down, length, corner));
}

FVector midpoint(FVector a, FVector b) {
	return FVector((a.X + b.X) / 2, (a.Y + b.Y) / 2, (a.Z + b.Z) / 2);
}

TArray<FVector> triforce_of_triangle(int32 upside_down, float length, FVector corner) {
	return equilateral_triangle(upside_down*-1, length/2.0, midpoint(corner, high_corner(upside_down, length, corner)));
}

TArray<FVector> generate_triangles() {
	triangle(FVector(1000, 0, 0), FVector(0, 0, 0), FVector(500, 0, 1000));
	return triangle(FVector(500, 0, 0), FVector(0, 0, 0), FVector(500, 0, 1000));
}



// Sets default values
AProcTriangle::AProcTriangle()
{
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;

	UProceduralMeshComponent* mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	TArray<FVector> p = equilateral_triangle(1,1000.0, FVector(0, 0, 0));
	p.Append(triforce_of_triangle(1, 1000.0, FVector(0, 0, 0)));
	p.Append(triforce_of_triangle(1, 1000.0/2, FVector(0, 0, 0)));
	p.Append(triforce_of_triangle(1, 1000.0/4, FVector(0, 0, 0)));
	p.Append(triforce_of_triangle(1, 1000.0/8, FVector(0, 0, 0)));
	p.Append(triforce_of_triangle(1, 1000.0/16, FVector(0, 0, 0)));



	TArray<int32> triangles = all_triangles(3*5);


	// With default options
	//mesh->CreateMeshSection(1, vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	mesh->CreateMeshSection(1, p, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);


	mesh->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void AProcTriangle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProcTriangle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

