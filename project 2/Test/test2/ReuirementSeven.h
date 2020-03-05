template <typename T>
Repository* MetricsAnalysis<T>::parse(std::string pa)
{
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	if (pParser)
	{
		if (!configure.Attach(pa))
		{
			std::cout << "\n  could not open file " << pa << std::endl;
		}
	}
	else
		std::cout << "\n\n  Parser not built\n\n";
	// now that parser is built, use it
	while (pParser->next())
		pParser->parse();
	std::cout << "\n\n";
}