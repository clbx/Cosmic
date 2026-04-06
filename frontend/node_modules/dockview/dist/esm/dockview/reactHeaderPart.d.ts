import React from 'react';
import { ReactPortalStore } from '../react';
import { PanelUpdateEvent, ITabRenderer, TabPartInitParameters, IDockviewPanelHeaderProps } from 'dockview-core';
export declare class ReactPanelHeaderPart implements ITabRenderer {
    readonly id: string;
    private readonly component;
    private readonly reactPortalStore;
    private readonly _element;
    private part?;
    get element(): HTMLElement;
    constructor(id: string, component: React.FunctionComponent<IDockviewPanelHeaderProps>, reactPortalStore: ReactPortalStore);
    focus(): void;
    init(parameters: TabPartInitParameters): void;
    update(event: PanelUpdateEvent): void;
    layout(_width: number, _height: number): void;
    dispose(): void;
}
