#pragma once

#include <type_traits>
#include <vector>

#include "conquest/graphics/widgets/widget.h"

namespace conquest::gui {

	/**
	 * The type of motions supported by container.
	 */
	enum class Motion
	{
		Horizontal,
		Vertical
	};

	/**
	 * Marks a widget a widget that can be focused on.
	 */
	template<typename WidgetT>
	struct Focus
	{
		static_assert(std::is_base_of_v<IWidget, WidgetT>, "Focusable objects can only be IInputWidget");

		WidgetT *widget;

		explicit Focus(WidgetT& widget)
			: widget(&widget)
		{}
	};

	/**
	 *  A widget that contains a lot of sub widgets.
	 */
	class Container : public IWidget,
					  public IInputWidget
	{
	public:
		/**
		 * A class that help control the directional motion in the container.
		 * Should be specialized for every conquest::gui::Motion type.
		 */
		template<Motion MotionType>
		struct Controller;

		/**
		 * Adds a normal widget to the container.
		 *
		 * @param widget - The widget to add.
		 */
		void add(IWidget& widget);

		/**
		 * Adds a widget that can be focused on to the container.
		 *
		 * @param focusableWidget - The widget to add.
		 */
		template<typename WidgetT>
		void add(Focus<WidgetT> focusableWidget)
		{
			mFocusList.push_back(focusableWidget.widget);
			add(std::make_pair(static_cast<int32>(mFocusList.size() - 1), focusableWidget.widget));
		}

		virtual void draw(Screen& screen, bool highlighted);

		virtual bool sendInput(const InputResult& input);

	private:
		template<Motion>
		friend struct Controller;

		void add(std::pair<int32, IWidget *> widget);

		/**
		 * Increments the pointer, and returns if it was actually incremented.
		 */
		bool increment();
		/**
		 * Decrements the pointer, and returns if it was actually decremeneted.
		 */
		bool decrement();

		int32 mPointer = 0;
		std::vector<std::pair<int32, IWidget *>> mWidgets; // The widgets contained in the object.
		std::vector<IWidget *> mFocusList;				   // The widgets that be focused on.
	};
}

/**
 * Horizontal GUI control specialization.
 */
template<>
struct conquest::gui::Container::Controller<conquest::gui::Motion::Horizontal>
{
	static bool handle(const InputResult& input, Container& container)
	{
		if(InputType::Left == input.type) {
			return container.decrement();
		}

		if(InputType::Right == input.type) {
			return container.increment();
		}

		return false;
	}
};

/**
 * Vertical GUI control specialization
 */
template<>
struct conquest::gui::Container::Controller<conquest::gui::Motion::Vertical>
{
	static bool handle(const InputResult& input, Container& container)
	{
		if(InputType::Up == input.type) {
			return container.decrement();
		}

		if(InputType::Down == input.type) {
			return container.increment();
		}

		return false;
	}
};

namespace conquest::gui {
	/**
	 * A container that can be controlled with the left and right arrows.
	 */
	class HorizontalContainer : public Container
	{
	public:
		virtual bool sendInput(const InputResult& input)
		{
			if(Container::sendInput(input)) {
				return true;
			}

			return Container::Controller<Motion::Horizontal>::handle(input, *this);
		}
	};

	/**
	 * A container that can be controlled with the up na down arrows.
	 */
	class VerticalContainer : public Container
	{
	public:
		virtual bool sendInput(const InputResult& input)
		{
			if(Container::sendInput(input)) {
				return true;
			}

			return Container::Controller<Motion::Vertical>::handle(input, *this);
		}
	};
}
