#include "stdafx.h"
#include "Animation.h"
#include <stdio.h>
#include <fstream>

void Animation::Initialize(char *filename)
{
  FILE *fp = NULL;
  fopen_s(&fp, filename, "r");


}