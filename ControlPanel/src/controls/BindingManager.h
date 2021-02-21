#ifndef __ELITEDUINO_BINDING_MANAGER_H__
#define __ELITEDUINO_BINDING_MANAGER_H__

namespace Eliteduino
{
	namespace Controls
	{
		union Binding;

		class BindingsManager
		{
		public:
			void Initialize( uint8_t numRows, uint8_t numCols );

			bool AreStoredBindingsValid() const;

			const Binding* GetBinding( uint8_t buttonIndex ) const;

		private:
			uint32_t CalculateCRC() const;
			static uint32_t CalculateCRC( uint16_t start, uint16_t end );

			void StoreDefaultBindings();
			void LoadBindings();

			Binding* m_bindings;
			uint8_t m_buttonCount = 0;
		};
	}
}

#endif // __ELITEDUINO_BINDING_MANAGER_H__
