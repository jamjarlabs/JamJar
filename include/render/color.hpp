#ifndef RENDER_COLOR_HPP
#define RENDER_COLOR_HPP

namespace JamJar {
class Color {
  public:
    Color();
    explicit Color(double red, double green, double blue);
    explicit Color(double red, double green, double blue, double alpha);
    double m_red;
    double m_green;
    double m_blue;
    double m_alpha;
};
}; // namespace JamJar

#endif
