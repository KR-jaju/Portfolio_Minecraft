#pragma once

struct CameraMatrices
{
	Mat view;
	Mat projection;
	Mat view_projection;
	Mat view_inverse_transpose;
	Mat	projection_inverse;
};