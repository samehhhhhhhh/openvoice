using CommunityToolkit.Mvvm.ComponentModel;

namespace openvoiceDesktop.ViewModels;

public partial class MainViewModel : ViewModelBase
{
    [ObservableProperty]
    public partial string Greeting { get; set; } = "Welcome to openvoice !";
}
