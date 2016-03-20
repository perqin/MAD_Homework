using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace MAD_HW4.ViewModels
{
    /// <summary>
    /// Store view state data for MainPage.
    /// </summary>
    public class MainAdaptiveViewModel : INotifyPropertyChanged
    {
        public bool ShowNewButton
        {
            get
            {
                return showNewButton;
            }
            set
            {
                showNewButton = value;
                OnPropertyChanged();
            }
        }
        public bool ShowBackButton
        {
            get
            {
                return showBackButton;
            }
            set
            {
                showBackButton = value;
                OnPropertyChanged();
            }
        }
        public bool ShowSaveResetDeleteButton
        {
            get
            {
                return showSaveResetDeleteButton;
            }
            set
            {
                showSaveResetDeleteButton = value;
                OnPropertyChanged();
            }
        }
        public bool ShowEditFrame
        {
            get
            {
                return showEditFrame;
            }
            set
            {
                showEditFrame = value;
                OnPropertyChanged();
            }
        }
        public ScreenWidthEnum ScreenWidth
        {
            get
            {
                return screenWidth;
            }
            set
            {
                screenWidth = value;
                adapt();
            }
        }
        public int SelectedItemIndex
        {
            get
            {
                return selectedItemIndex;
            }
            set
            {
                selectedItemIndex = value;
                adapt();
            }
        }

        public event PropertyChangedEventHandler PropertyChanged = delegate { };

        public override string ToString()
        {
            //TODO: Convert it to string
            return SelectedItemIndex.ToString();
        }

        public void FromString(string data)
        {
            //TODO: Convert it to VM
            if (data != null)
            {
                SelectedItemIndex = int.Parse(data);
            }
        }

        private bool showNewButton = true;
        private bool showBackButton = false;
        private bool showSaveResetDeleteButton = false;
        private bool showEditFrame = false;
        private ScreenWidthEnum screenWidth = ScreenWidthEnum.Wide;
        private int selectedItemIndex = -1;

        private void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        /// State table
        /// 	                narrow		                wide	
        ///             selected    no selected     selected    no selected
        /// new btn	        -	        +	            +	        +
        /// save btn        +	        -	            +	        -
        /// reset btn       +	        -	            +	        -
        /// delete btn      +	        -	            +	        -
        /// back button     +	        -	            -	        -
        private void adapt()
        {
            ShowNewButton = ScreenWidth == ScreenWidthEnum.Wide || SelectedItemIndex == -1;
            ShowSaveResetDeleteButton = SelectedItemIndex != -1;
            ShowBackButton = ScreenWidth == ScreenWidthEnum.Narrow && SelectedItemIndex != -1;
            ShowEditFrame = SelectedItemIndex != -1;
        }
    }

    public enum ScreenWidthEnum { Wide, Narrow };
}
