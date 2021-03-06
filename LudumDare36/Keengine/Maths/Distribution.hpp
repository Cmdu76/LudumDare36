#ifndef KE_DISTRIBUTION_HPP
#define KE_DISTRIBUTION_HPP

#include <cassert>
#include <functional>
#include <type_traits>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Random.hpp"
#include "Vector2.hpp"

namespace ke
{

namespace priv
{
	template <typename T>
	struct Constant
	{
		explicit Constant(T value)
			: value(value)
		{
		}

		T operator() () const
		{
			return value;
		}

		T value;
	};
} // namespace priv

template <typename T>
class Distribution
{
	public:
        Distribution(T constant)
        : mFactory(priv::Constant<T>(constant))
		{
		}

        Distribution(std::function<T()> function)
		: mFactory(function)
		{
		}

		T operator()() const
		{
			return mFactory();
		}

	private:
		std::function<T()> mFactory;
};

namespace Distributions
{

Distribution<int> uniform(int min, int max);
Distribution<unsigned int> uniform(unsigned int min, unsigned int max);
Distribution<float> uniform(float min, float max);
Distribution<sf::Time> uniform(sf::Time min, sf::Time max);
Distribution<sf::Vector2f> rect(sf::Vector2f center, sf::Vector2f halfSize);
Distribution<sf::Vector2f> circle(sf::Vector2f center, float radius);
Distribution<sf::Vector2f> deflect(sf::Vector2f direction, float maxRotation);

template <typename T>
Distribution<T> uniformT(T min, T max)
{
    assert(min <= max);

    return Distribution<T>([=] () -> T
    {
        return random(min, max);
    });
}

} // namespace Distributions

} // namespace ke

#endif // KE_DISTRIBUTION_HPP
