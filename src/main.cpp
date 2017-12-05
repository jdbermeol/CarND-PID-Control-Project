#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include "Twiddle.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid_s, pid_t;
  pid_s.Init(0.163011, 0.00031952, 3.50027);
  pid_t.Init(0.348404, 0.0, 0.0226185);
  
  Twiddle twiddle;
  std::vector<double> twiddle_params = {
    pid_s.Kp, pid_s.Ki, pid_s.Kd, pid_t.Kp, pid_t.Ki, pid_t.Kd};
  twiddle.Init(0.1, twiddle_params);

  double acc_err = 0.0;
  int count = 0;

  h.onMessage([&pid_s, &pid_t, &twiddle, &acc_err, &count](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double steer_value, throttle;

          pid_s.UpdateError(cte);
          steer_value = pid_s.TotalError();
          steer_value = std::max(-1.0, std::min(1.0, steer_value));

          pid_t.UpdateError(std::fabs(cte));
          throttle = 0.7 + pid_t.TotalError();
          throttle = 0.2;

          // if(count == 128) {
          //   double err = acc_err / 128.0;

          //   if(!twiddle.Converged()) {
          //     std::vector<double> params = twiddle.Update(err);
          //     pid_s.Kp = params[0];
          //     pid_s.Ki = params[1];
          //     pid_s.Kd = params[2];
          //     pid_t.Kp = params[3];
          //     pid_t.Ki = params[4];
          //     pid_t.Kd = params[5];
          //   }
          //   std::cout << pid_s.Kp << " " << pid_s.Ki << " " << pid_s.Kd << " " << pid_t.Kp << " " << pid_t.Ki << " " << pid_t.Kd << std::endl;            

          //   acc_err = 0.0;
          //   count = 0;
          // } else {
          //   count += 1;
          //   acc_err += std::fabs(cte);
          // }

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
