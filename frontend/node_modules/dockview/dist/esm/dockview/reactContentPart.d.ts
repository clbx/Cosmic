import React from 'react';
import { ReactPortalStore } from '../react';
import { DockviewEvent, PanelUpdateEvent, IContentRenderer, GroupPanelPartInitParameters, IDockviewPanelProps } from 'dockview-core';
export declare class ReactPanelContentPart implements IContentRenderer {
    readonly id: string;
    private readonly component;
    private readonly reactPortalStore;
    private readonly _element;
    private part?;
    private readonly _onDidFocus;
    readonly onDidFocus: DockviewEvent<void>;
    private readonly _onDidBlur;
    readonly onDidBlur: DockviewEvent<void>;
    get element(): HTMLElement;
    constructor(id: string, component: React.FunctionComponent<IDockviewPanelProps>, reactPortalStore: ReactPortalStore);
    focus(): void;
    init(parameters: GroupPanelPartInitParameters): void;
    update(event: PanelUpdateEvent): void;
    layout(_width: number, _height: number): void;
    dispose(): void;
}
