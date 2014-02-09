/******************************************************************************
 * test_tps.cpp
 *
 * Project:  thinplatespline
 * Purpose:  Simple test example of 2D Thin Plate Spline transformer.
 * Author:   Ko Nagase, geosanak@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2011, Omniscale GmbH & Co. KG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

#include <stdio.h>
#include "tps.h"

int main(int args, char* argv[])
{
    // test_init_from_list
    {
        double points[2][4] = {{0, 0, 50, 50}, {10, 10, 100, 100}};
        TPS* t = new TPS((double*)points, 2);
        double dst[2];
        t->transform(4, 5, dst);
        if (!(dst[0] == 72.5 && dst[1] == 72.5))
        {
            // TODO:
            printf("test_init_from_list-1: failed\n");
        }
        t->add(0, 10, 70, 100);
        t->transform(4, 5, dst);
        if (!(dst[0] == 72.0 && dst[1] == 75.0))
        {
            // TODO:
            printf("test_init_from_list-2:failed\n");
        }
        delete t;
    }
    // test_simple
    {
        TPS* t = new TPS();
        t->add(0, 0, 50, 50);
        t->add(10, 10, 100, 100);
        double dst[2];
        t->transform(4, 5, dst);
        if (!(dst[0] == 72.5 && dst[1] == 72.5))
        {
            // TODO:
            printf("test_simple-1:failed\n");
        }
        t->add(0, 10, 70, 100);
        t->transform(4, 5, dst);
        if (!(dst[0] == 72.0 && dst[1] == 75.0))
        {
            // TODO:
            printf("test_simple-2:failed\n");
        }
        delete t;
    }
    // test_no_points
    {
        try
        {
            TPS* t = new TPS();
            double dst[2];
            t->transform(0, 0, dst);
            printf("test_no_points-1:failed\n");
        }
        catch (...)
        {
        }
    }
    // test_from_control_points_list
    {
        double points[3][4] = {{0, 0, 50, 50}, {10, 10, 100, 100}, {0, 10, 70, 100}};
        TPS* t = TPS::from_control_points((double*)points, 3);
        double dst[2];
        t->transform(4, 5, dst);
        if (!(dst[0] == 72.0 && dst[1] == 75.0))
        {
            // TODO:
            printf("test_from_control_points_list-1:failed\n");
        }
        delete t;
    }
    // test_from_control_points_list_backwards
    { 
        double points[3][4] = {{0, 0, 50, 50}, {10, 10, 100, 100}, {0, 10, 70, 100}};
        TPS* t = TPS::from_control_points((double*)points, 3, true);
        double dst[2];
        t->transform(72, 75, dst);
        if (!(dst[0] == 4.0 && dst[1] == 5.0))
        {
            // TODO:
            printf("test_from_control_points_list_backwards-1:failed\n");
        }
        delete t;
    }
    // test_tilemapjp
    {
        double points[3][4] = {{100, 100, 200, 200}, {200, 200, 400, 400}, {150, 150, 320, 350}};
        TPS* tps_f = TPS::from_control_points((double*)points, 3, false);
        TPS* tps_b = TPS::from_control_points((double*)points, 3, true);

        tps_f->solve();
        tps_b->solve();

        //Forward transform
        double ord[2];
        tps_f->transform(160, 160, ord);
        if (!(ord[0] == 336 && ord[1] == 360))
        {
            // TODO:
            printf("test_tilemapjp-1:failed\n");
        }

        //Backward transform
        double rev[2];
        tps_b->transform(ord[0], ord[1], rev);
        if (!(rev[0] == 160 && rev[1] == 160))
        {
            // TODO:
            printf("test_tilemapjp-2:failed\n");
        }

        //Solving thin-Plate-Spline from many points by scrach takes too many time.
        //So, there are object-serialization method to store solved instance.
        int serial_size;
        serial_size = tps_f->serialize_size();
        char* serial_f = new char[serial_size];
        tps_f->serialize(serial_f);
        serial_size = tps_b->serialize_size();
        char* serial_b = new char[serial_size];
        tps_b->serialize(serial_b);

        TPS* tps2_f = new TPS();
        tps2_f->deserialize(serial_f);
        TPS* tps2_b = new TPS();
        tps2_b->deserialize(serial_b);

        double ord2[2];
        tps2_f->transform(160, 160, ord2);
        double rev2[2];
        tps2_b->transform(ord2[0], ord2[1], rev2);
        //Same results with ord, rev
        if (!(ord[0] == ord2[0] && ord[1] == ord2[1]))
        {
            // TODO:
            printf("test_tilemapjp-3:failed\n");
        }
        if (!(rev[0] == rev2[0] && rev[1] == rev2[1]))
        {
            // TODO:
            printf("test_tilemapjp-4:failed\n");
        }

        delete [] serial_f;
        delete [] serial_b;
    }
    return 0;
}