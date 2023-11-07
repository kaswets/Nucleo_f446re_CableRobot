float acos1(float X)
{
    // return((-0.155972*x+1.56467)*sqrt(1.0f-x));

    float result;

    result = ((-0.155972 * X + 1.56467) * sqrt(1.0 - X));

    return result;
};