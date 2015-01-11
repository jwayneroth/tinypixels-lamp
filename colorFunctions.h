/*
Start with a temperature, in Kelvin, somewhere between 1000 and 40000.  (Other values may work,
but I can't make any promises about the quality of the algorithm's estimates above 40000 K.)
Note also that the temperature and color variables need to be declared as floating-point.
*/
void rgbFromTemp( uint32_t temp, int* rgbValues ) {

	float temperature = temp / 100;
	float red, green, blue;
	
	//Calculate red:
	if( temperature <= 66 ) {
			red = 255;
	}else{
			red = temperature - 60;
			red = 329.698727446 * (pow(red , -0.1332047592));
			if( red < 0 )  red = 0;
			if( red > 255 ) red = 255;
	}

	//Calculate green:
	if( temperature <= 66 ) {
			green = temperature;
			green = 99.4708025861 * log(green) - 161.1195681661;
			if( green < 0 ) green = 0;
			if( green > 255 ) green = 255;
	}else{
			green = temperature - 60;
			green = 288.1221695283 * (pow(green , -0.0755148492));
			if( green < 0 ) green = 0;
			if( green > 255 ) green = 255;
	}

	//Calculate blue:
	if( temperature >= 66 ) {
			blue = 255;
	}else{

		if( temperature <= 19 ){
			blue = 0;
		}else{
			blue = temperature - 10;
			blue = 138.5177312231 * log(blue) - 305.0447927307;
			if( blue < 0 ) blue = 0;
			if( blue > 255 ) blue = 255;
		}
	
	}
	
	rgbValues[0] = (int)red;
	rgbValues[1] = (int)green;
	rgbValues[2] = (int)blue;
	
}

/*
Start with a temperature, in Kelvin, somewhere between 1000 and 40000.  (Other values may work,
but I can't make any promises about the quality of the algorithm's estimates above 40000 K.)
Note also that the temperature and color variables need to be declared as floating-point.

Set temperature = temperature \ 100

Calculate red:

If temperature <= 66 Then
		red = 255
Else
		red = temperature - 60
		red = 329.698727446 * (red ^ -0.1332047592)
		If red < 0 Then red = 0
		If red > 255 Then red = 255
End If

Calculate green:

If temperature <= 66 Then
		green = temperature
		green = 99.4708025861 * Ln(green) - 161.1195681661
		If green < 0 Then green = 0
		If green > 255 Then green = 255
Else
		green = temperature - 60
		green = 288.1221695283 * (green ^ -0.0755148492)
		If green < 0 Then green = 0
		If green > 255 Then green = 255
End If

Calculate blue:

If temperature >= 66 Then
		blue = 255
Else

		If temperature <= 19 Then
				blue = 0
		Else
				blue = temperature - 10
				blue = 138.5177312231 * Ln(blue) - 305.0447927307
				If blue < 0 Then blue = 0
				If blue > 255 Then blue = 255
		End If

End If
*/